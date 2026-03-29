#pragma once

#include "lfs.h"
namespace Filesystem {
constexpr uint32_t FLASH_BLOCK_SIZE = 4096;
constexpr uint32_t FLASH_SECTOR_SIZE = 256;
constexpr uint32_t FLASH_SIZE = (64 * 1024 * 1024 / 8);

constexpr uint8_t READ_DATA = 0x0B;
constexpr uint8_t WRITE_ENABLE = 0x06;
constexpr uint8_t PAGE_PROGRAM = 0x02;
constexpr uint8_t SECTOR_ERASE = 0x20;
constexpr uint8_t READ_STATUS_REG = 0x05;
constexpr uint8_t DUMMY_BYTE = 0xFF;

/**
 * @brief LittleFS main object instance.
 */
extern lfs_t lfs;

/**
 * @brief Initialize the filesystem and mount LittleFS.
 * @return true if initialization succeeded, false otherwise.
 */
bool init();

/**
 * @brief Read data from flash.
 * @param c Pointer to lfs_config structure.
 * @param block Block number to read from.
 * @param off Offset within the block.
 * @param buffer Buffer to store read data.
 * @param size Number of bytes to read.
 * @return 0.
 */
int w25q64_read(const lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);

/**
 * @brief Write data to flash.
 * @param c Pointer to lfs_config structure.
 * @param block Block number to write to.
 * @param off Offset within the block.
 * @param buffer Buffer containing data to write.
 * @param size Number of bytes to write.
 * @return 0.
 */
int w25q64_prog(const lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);

/**
 * @brief Erase a block in flash.
 * @param c Pointer to lfs_config structure.
 * @param block Block number to erase.
 * @return 0.
 */
int w25q64_erase(const lfs_config *c, lfs_block_t block);

/**
 * @brief Dummy sync function; does nothing.
 * @param c Pointer to lfs_config structure (unused).
 * @return 0.
 */
int w25q64_sync(const lfs_config *c);
} // namespace Filesystem