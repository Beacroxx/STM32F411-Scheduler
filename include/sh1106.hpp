#pragma once

#include "types.hpp"

#include <libopencm3/stm32/gpio.h>

namespace SH1106 {
constexpr uint32_t WIDTH = 128;
constexpr uint32_t HEIGHT = 64;

constexpr uint8_t ADDR = 0x3C << 1;

constexpr uint8_t COMMAND = 0x00;
constexpr uint8_t DATA = 0x40;
constexpr uint8_t MULTI = 0x00;
constexpr uint8_t SINGLE = 0x80;

constexpr uint32_t PORT = GPIOB;
constexpr uint32_t SCL = GPIO8;
constexpr uint32_t SDA = GPIO9;

extern uint64_t buf[WIDTH];
extern const uint8_t init_seq[];

void init();
void clear();
void update();

void send(uint8_t byte);
void stop();
void begin();

void drawPixel(const Vec2 &v, bool white = true);
void drawLine(const Line2 &l, bool white = true);
void drawFastVLine(const Vec2 &v, uint32_t l, bool white = true);
void drawFastHLine(const Vec2 &v, uint32_t l, bool white = true);
void drawTriangle(const Tri2 &t, bool white = true);
void drawFastRect(const Rect2 &r, bool fill = false, bool white = true);
void drawText(const Vec2 &v, const char *s);
} // namespace SH1106