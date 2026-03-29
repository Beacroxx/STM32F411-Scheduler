#pragma once

/**
 * @brief Read a line from standard input.
 * @param buf Buffer to store the input line.
 * @param maxlen Maximum number of characters to read.
 * @return Number of characters read.
 */
int readline(char *buf, int maxlen);

/**
 * @brief Write data to a file descriptor.
 * @param file File descriptor.
 * @param ptr Pointer to data buffer.
 * @param len Number of bytes to write.
 * @return Number of bytes written.
 */
extern "C" int _write(int file, char *ptr, int len);

/**
 * @brief Read data from a file descriptor.
 * @param file File descriptor.
 * @param ptr Pointer to buffer to fill.
 * @param len Number of bytes to read.
 * @return Number of bytes read.
 */
extern "C" int _read(int file, char *ptr, int len);