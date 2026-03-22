#pragma once

#include "lfs.h"

#define FLASH_BLOCK_SIZE  4096
#define FLASH_SECTOR_SIZE 256
#define FLASH_SIZE        (64 * 1024 * 1024 / 8) // 64 Mbit

namespace Filesystem {
extern lfs_config w25q64_lfs_cfg;
extern lfs_t lfs;

bool init();

int w25q64_read(const lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);
int w25q64_prog(const lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);
int w25q64_erase(const lfs_config *c, lfs_block_t block);
int w25q64_sync(const lfs_config *c);
} // namespace Filesystem