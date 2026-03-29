#include "sh1106.hpp"

#include "font.hpp"
#include "malloc.hpp"
#include "systick.hpp"

#include <libopencm3/stm32/rcc.h>

uint64_t SH1106::buf[WIDTH];

const uint8_t SH1106::init_seq[] = {
    0xAE,       // Display off
    0xD5, 0xF0, // Set display clock divide ratio/oscillator frequency
    0xA8, 0x3F, // Set multiplex ratio
    0xD3, 0x00, // Set display offset
    0x40,       // Set display start line
    0x8D, 0x14, // Enable charge pump
    0x20, 0x02, // Set memory addressing mode
    0xA0,       // Set segment re-map
    0xC8,       // Set COM output direction
    0xDA, 0x12, // Set COM pins hardware configuration
    0x81, 0xFF, // Set contrast control
    0xD9, 0x11, // Set pre-charge period
    0xDB, 0x20, // Set VCOMH deselect level (0.77 * Vcc)
    0xA4,       // Display follows RAM content
    0xA6,       // Normal display
    0xAF,       // Display on
};

void SH1106::send(uint8_t byte) {
  uint32_t i = 8;
  do {
    if (byte & 0x80)
      gpio_set(PORT, SDA);
    else
      gpio_clear(PORT, SDA);

    gpio_set(PORT, SCL);
    SysTick::delayCyc(1);
    gpio_clear(PORT, SCL);
    byte <<= 1;
  } while (i--);
}

void SH1106::stop() {
  gpio_set(PORT, SCL);
  gpio_set(PORT, SDA);
}

void SH1106::begin() {
  gpio_clear(PORT, SDA);
  gpio_clear(PORT, SCL);
  send(ADDR);
}

void SH1106::update() {
  for (int32_t p = 7; p >= 0; p--) {
    begin();
    send(COMMAND | SINGLE);
    send(0xB0 | p);
    send(COMMAND | SINGLE);
    send(0x02);
    send(COMMAND | SINGLE);
    send(0x10);
    send(DATA | MULTI);
    uint32_t i = 127;
    do
      send(buf[i] >> (p * 8));
    while (i--);
    stop();
  }
}

void SH1106::clear() { Malloc::memset(buf, 0x00, 128 * sizeof(uint64_t)); }

void SH1106::init() {
  gpio_mode_setup(PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SDA | SCL);
  gpio_set(PORT, SDA | SCL);

  begin();
  send(COMMAND | MULTI);
  uint32_t i = 0;
  do
    send(init_seq[i]);
  while (i++ < sizeof(init_seq));
  stop();

  clear();
  update();
};

void SH1106::drawPixel(const Vec2 &v, bool white) {
  int32_t x = static_cast<int32_t>(v.x);
  int32_t y = static_cast<int32_t>(v.y);

  if (x < 0 || y < 0 || x >= static_cast<int32_t>(WIDTH) || y >= static_cast<int32_t>(HEIGHT))
    return;

  buf[x] = (buf[x] & ~(1ull << y)) | (((white & 1ull) << y));
}

void SH1106::drawLine(const Line2 &l, bool white) {
  int32_t x1 = static_cast<int32_t>(l.v1.x);
  int32_t y1 = static_cast<int32_t>(l.v1.y);
  int32_t x2 = static_cast<int32_t>(l.v2.x);
  int32_t y2 = static_cast<int32_t>(l.v2.y);

  int32_t dx = abs(x2 - x1);
  int32_t dy = abs(y2 - y1);
  int32_t sx = (x1 < x2) ? 1 : -1;
  int32_t sy = (y1 < y2) ? 1 : -1;
  int32_t err = (dx > dy ? dx : -dy) / 2;
  int32_t e2;

  while (true) {
    drawPixel(Vec2(x1, y1), white);

    if (x1 == x2 && y1 == y2)
      break;
    e2 = err;
    if (e2 > -dx) {
      err -= dy;
      x1 += sx;
    }
    if (e2 < dy) {
      err += dx;
      y1 += sy;
    }
  }
}

void SH1106::drawTriangle(const Tri2 &t, bool white) {
  drawLine(Line2(t.v1, t.v2), white);
  drawLine(Line2(t.v2, t.v3), white);
  drawLine(Line2(t.v3, t.v1), white);
}

void SH1106::drawFastVLine(const Vec2 &v, uint32_t l, bool white) { drawFastRect(Rect2(v, Vec2(1, l)), false, white); }
void SH1106::drawFastHLine(const Vec2 &v, uint32_t l, bool white) { drawFastRect(Rect2(v, Vec2(l, 1)), false, white); }

void SH1106::drawFastRect(const Rect2 &r, bool fill, bool white) {
  int32_t x = static_cast<int32_t>(r.xy.x);
  int32_t y = static_cast<int32_t>(r.xy.y);
  int32_t w = static_cast<int32_t>(r.wh.x);
  int32_t h = static_cast<int32_t>(r.wh.y);

  if (w + x <= 0 || h + y <= 0 || x >= static_cast<int32_t>(WIDTH) || y >= static_cast<int32_t>(HEIGHT))
    return;

  if (x < 0) {
    w += x;
    x = 0;
  }

  if (y < 0) {
    h += y;
    y = 0;
  }

  if (x + w > static_cast<int32_t>(WIDTH))
    w -= WIDTH - x;
  if (y + h > static_cast<int32_t>(HEIGHT))
    h -= HEIGHT - y;

  --w;

  uint64_t mask = ((1ull << h) - 1) << y;
  uint64_t val = -static_cast<uint64_t>(white) & mask;

  buf[x] = (buf[x] & ~mask) | val;
  if (w == 0)
    return;
  buf[x + w] = (buf[x + w] & ~mask) | val;
  if (w == 1)
    return;

  if (!fill && h > 2) {
    mask = ((1ull << (h - 1)) | 1ull) << y;
    val = -static_cast<uint64_t>(white) & mask;
  }

  while (--w)
    buf[x + w] = (buf[x + w] & ~mask) | val;
}

void SH1106::drawText(const Vec2 &v, const char *s) {
  int32_t x = static_cast<int32_t>(v.x);
  int32_t y = static_cast<int32_t>(v.y);

  while (uint32_t c = *s++) {
    if (c == '\n') {
      y += 8;
      x = static_cast<int32_t>(v.x);
      continue;
    }

    if (x < 0 || y < 0 || x >= static_cast<int32_t>(WIDTH) - 5 || y >= static_cast<int32_t>(HEIGHT) - 7) {
      x += 6;
      continue;
    }

    c *= 5;
    for (uint32_t i = 0; i <= 4; i++) {
      buf[x++] |= static_cast<uint64_t>(font_5x7[c++]) << y;
    }
    x++;
  }
}