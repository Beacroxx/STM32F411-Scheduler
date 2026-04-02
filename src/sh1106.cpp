#include "sh1106.hpp"

#include "font.hpp"
#include "memorymanager.hpp"
#include "systick.hpp"

#include <libopencm3/cm3/cortex.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

uint64_t SH1106::buf[WIDTH];

// clang-format off
const uint8_t SH1106::init_seq[] = {
  CMD::SET_DISPLAY_OFF,
  CMD::SET_DISPLAY_CLOCK_DIVIDE, 0xF0,
  CMD::SET_MULTIPLEX_RATIO, 0x3F,
  CMD::SET_DISPLAY_OFFSET, 0x00,
  CMD::SET_DISPLAY_START_LINE,
  CMD::ENABLE_CHARGE_PUMP, 0x14,
  CMD::SET_MEMORY_ADDRESSING_MODE, 0x02,
  CMD::SET_SEGMENT_REMAP,
  CMD::SET_COM_OUTPUT_SCAN_DIRECTION_REMAP,
  CMD::SET_COM_PINS_HARDWARE_CONFIGURATION, 0x12,
  CMD::SET_CONTRAST_CONTROL, 0xFF,
  CMD::SET_PRECHARGE_PERIOD, 0x11,
  CMD::SET_VCOMH_DESELECT_LEVEL, 0x20,
  CMD::DISPLAY_FOLLOW_RAM,
  CMD::NORMAL_DISPLAY,
  CMD::SET_DISPLAY_ON,
};
// clang-format on

void SH1106::send(uint8_t byte) {
  uint32_t i = 8;
  do {
    GPIO_BSRR(PORT) = byte & 0x80 ? SDA : SDA << 16;
    GPIO_BSRR(PORT) = SCL;
    SysTick::delayCyc(3);
    GPIO_BSRR(PORT) = SCL << 16;
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
  for (int32_t p = 0; p != 8; p++) {
    cm_disable_interrupts();
    begin();
    send(COMMAND | SINGLE);
    send(CMD::SET_PAGE_ADDRESS_BASE | p);
    send(COMMAND | SINGLE);
    send(CMD::SET_COLUMN_LOW | 0x02);
    send(COMMAND | SINGLE);
    send(CMD::SET_COLUMN_HIGH | 0x00);
    send(DATA | MULTI);
    for (int32_t i = 127; i >= 0; i--)
      send(buf[i] >> (p * 8));
    stop();
    cm_enable_interrupts();
  }
}

void SH1106::clear() { MM::memset(buf, 0x00, 128 * sizeof(uint64_t)); }

void SH1106::init() {
  gpio_mode_setup(PORT, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, SDA | SCL);
  gpio_set(PORT, SDA | SCL);

  begin();
  send(COMMAND | MULTI);
  uint32_t i = 0;
  do
    send(init_seq[i]);
  while (++i < sizeof(init_seq));
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

void SH1106::drawTriangle(const Tri2 &t, bool fill, bool white) {
  if (!fill) {
    drawLine(Line2(t.v1, t.v2), white);
    drawLine(Line2(t.v2, t.v3), white);
    drawLine(Line2(t.v3, t.v1), white);
    return;
  }

  const i16f16_t x1 = t.v1.x, x2 = t.v2.x, x3 = t.v3.x;
  i16f16_t min_x = x1 < x2 ? (x1 < x3 ? x1 : x3) : (x2 < x3 ? x2 : x3);
  i16f16_t max_x = x1 > x2 ? (x1 > x3 ? x1 : x3) : (x2 > x3 ? x2 : x3);

  int32_t bxMin = static_cast<int32_t>(min_x);
  int32_t bxMax = static_cast<int32_t>(max_x + i16f16_t(1));

  const i16f16_t y1 = t.v1.y, y2 = t.v2.y, y3 = t.v3.y;

  for (int32_t x = bxMin; x <= bxMax; ++x) {
    const i16f16_t fx = i16f16_t(x) + i16f16_t(0.5f);

    i16f16_t ymin = i16f16_t(-32768);
    i16f16_t ymax = i16f16_t(-32768);

    if (x2 != x1)
      if (((x1 <= fx) & (x2 > fx)) | ((x2 <= fx) & (x1 > fx)))
        ymin = y1 + (y2 - y1) * ((fx - x1) / (x2 - x1));

    if (x3 != x2)
      if (((x2 <= fx) & (x3 > fx)) | ((x3 <= fx) & (x2 > fx))) {
        i16f16_t v = y2 + (y3 - y2) * ((fx - x2) / (x3 - x2));
        if (ymin == i16f16_t(-32768))
          ymin = v;
        else {
          ymax = (v > ymin) ? v : ymin;
          ymin = (v > ymin) ? ymin : v;
        }
      }
    if (x1 != x3)
      if (((x3 <= fx) & (x1 > fx)) | ((x1 <= fx) & (x3 > fx))) {
        i16f16_t v = y3 + (y1 - y3) * ((fx - x3) / (x1 - x3));
        ymax = (v > ymin) ? v : ymin;
        ymin = (v > ymin) ? ymin : v;
      }

    if (ymin == i16f16_t(-32768) || ymax == i16f16_t(-32768) || ymin == ymax)
      continue;

    int32_t sy = static_cast<int32_t>(ymin + i16f16_t(0.5f));
    int32_t ey = static_cast<int32_t>(ymax + i16f16_t(0.5f));
    drawFastVLine(Vec2(x, sy), ey - sy, white);
  }
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

void SH1106::drawCenteredText(const Rect2 &r, const char *s) {
  uint32_t len = 0;
  for (const char *c = s; *c; c++)
    len++;

  drawText(Vec2(r.xy.x + (r.wh.x / 2 - len / 2), r.xy.y + (r.wh.y / 2 - 2)), s);
}

void SH1106::setMode(Mode mode) {
  begin();
  send(COMMAND | MULTI);
  switch (mode) {
  case Mode::NORMAL:
    send(CMD::NORMAL_DISPLAY);
    send(CMD::SET_SEGMENT_REMAP);
    send(CMD::SET_COM_OUTPUT_SCAN_DIRECTION_REMAP);
    send(CMD::SET_CONTRAST_CONTROL);
    send(0xFF);
    break;
  case Mode::INVERTED:
    send(CMD::INVERT_DISPLAY);
    break;
  case Mode::FLIP_H:
    send(CMD::NORMAL_DISPLAY);
    send(CMD::SET_SEGMENT_REMAP_REVERSE);
    break;
  case Mode::FLIP_V:
    send(CMD::NORMAL_DISPLAY);
    send(CMD::SET_COM_OUTPUT_SCAN_DIRECTION_NORMAL);
    break;
  case Mode::ROTATE_180:
    send(CMD::NORMAL_DISPLAY);
    send(CMD::SET_SEGMENT_REMAP_REVERSE);
    send(CMD::SET_COM_OUTPUT_SCAN_DIRECTION_NORMAL);
    break;
  case Mode::DIM:
    send(CMD::NORMAL_DISPLAY);
    send(CMD::SET_CONTRAST_CONTROL);
    send(0x40);
    break;
  case Mode::OFF:
    send(CMD::SET_DISPLAY_OFF);
    break;
  case Mode::ON:
    send(CMD::SET_DISPLAY_ON);
    break;
  default:
    break;
  }
  stop();
}