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

namespace CMD {
constexpr uint8_t SET_DISPLAY_OFF = 0xAE;
constexpr uint8_t SET_DISPLAY_ON = 0xAF;
constexpr uint8_t SET_DISPLAY_CLOCK_DIVIDE = 0xD5;
constexpr uint8_t SET_MULTIPLEX_RATIO = 0xA8;
constexpr uint8_t SET_DISPLAY_OFFSET = 0xD3;
constexpr uint8_t SET_DISPLAY_START_LINE = 0x40;
constexpr uint8_t ENABLE_CHARGE_PUMP = 0x8D;
constexpr uint8_t SET_MEMORY_ADDRESSING_MODE = 0x20;
constexpr uint8_t SET_SEGMENT_REMAP = 0xA0;
constexpr uint8_t SET_SEGMENT_REMAP_REVERSE = 0xA1;
constexpr uint8_t SET_COM_OUTPUT_SCAN_DIRECTION_NORMAL = 0xC0;
constexpr uint8_t SET_COM_OUTPUT_SCAN_DIRECTION_REMAP = 0xC8;
constexpr uint8_t SET_COM_PINS_HARDWARE_CONFIGURATION = 0xDA;
constexpr uint8_t SET_CONTRAST_CONTROL = 0x81;
constexpr uint8_t SET_PRECHARGE_PERIOD = 0xD9;
constexpr uint8_t SET_VCOMH_DESELECT_LEVEL = 0xDB;
constexpr uint8_t DISPLAY_FOLLOW_RAM = 0xA4;
constexpr uint8_t NORMAL_DISPLAY = 0xA6;
constexpr uint8_t INVERT_DISPLAY = 0xA7;
constexpr uint8_t SET_PAGE_ADDRESS_BASE = 0xB0;
constexpr uint8_t SET_COLUMN_LOW = 0x00;
constexpr uint8_t SET_COLUMN_HIGH = 0x10;
} // namespace CMD

enum class Mode {
  NORMAL,
  INVERTED,
  FLIP_H,     // Horizontal mirror (segment remap)
  FLIP_V,     // Vertical mirror (COM scan direction)
  ROTATE_180, // Equivalent to FLIP_H + FLIP_V
  DIM,        // Lower contrast
  OFF,        // Display off
  ON,         // Display on
};

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
 * @brief Switch between modes
 * @param mode The mode to switch to
 */
void setMode(Mode mode);

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
 * @param fill Fill the triangle.
 * @param white True for white, false for black.
 */
void drawTriangle(const Tri2 &t, bool fill = false, bool white = true);

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

/**
 * @brief Draw centered text inside the given rectangle.
 * @param r Rectangle to draw inside.
 * @param s Null-terminated string to draw.
 */
void drawCenteredText(const Rect2 &r, const char *s);
} // namespace SH1106