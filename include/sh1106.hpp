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

/**
 * @brief Display buffer.
 */
extern uint64_t buf[WIDTH];

/**
 * @brief Initialization command sequence.
 */
extern const uint8_t init_seq[];

/**
 * @brief Initialize the SH1106 OLED display.
 */
void init();

/**
 * @brief Clear the display buffer.
 */
void clear();

/**
 * @brief Update the display with the current buffer.
 */
void update();

/**
 * @brief Send a byte to the display.
 * @param byte Byte to send.
 */
void send(uint8_t byte);

/**
 * @brief Stop communication with the display.
 */
void stop();

/**
 * @brief Begin communication with the display.
 */
void begin();

/**
 * @brief Draw a pixel at the given position.
 * @param v Pixel position.
 * @param white True for white, false for black.
 */
void drawPixel(const Vec2 &v, bool white = true);

/**
 * @brief Draw a line between two points.
 * @param l Line to draw.
 * @param white True for white, false for black.
 */
void drawLine(const Line2 &l, bool white = true);

/**
 * @brief Draw a fast vertical line.
 * @param v Start position.
 * @param l Length of the line.
 * @param white True for white, false for black.
 */
void drawFastVLine(const Vec2 &v, uint32_t l, bool white = true);

/**
 * @brief Draw a fast horizontal line.
 * @param v Start position.
 * @param l Length of the line.
 * @param white True for white, false for black.
 */
void drawFastHLine(const Vec2 &v, uint32_t l, bool white = true);

/**
 * @brief Draw a triangle.
 * @param t Triangle to draw.
 * @param white True for white, false for black.
 */
void drawTriangle(const Tri2 &t, bool white = true);

/**
 * @brief Draw a rectangle.
 * @param r Rectangle to draw.
 * @param fill Fill the rectangle.
 * @param white True for white, false for black.
 */
void drawFastRect(const Rect2 &r, bool fill = false, bool white = true);

/**
 * @brief Draw text at the given position.
 * @param v Start position.
 * @param s Null-terminated string to draw.
 */
void drawText(const Vec2 &v, const char *s);
} // namespace SH1106