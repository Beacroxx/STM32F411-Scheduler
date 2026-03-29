#include "filesystem.hpp"

#include "spi.hpp"

lfs_t Filesystem::lfs;

lfs_config Filesystem::w25q64_lfs_cfg = {
    .context = NULL,
    .read = w25q64_read,
    .prog = w25q64_prog,
    .erase = w25q64_erase,
    .sync = w25q64_sync,
    .read_size = FLASH_SECTOR_SIZE,
    .prog_size = FLASH_SECTOR_SIZE,
    .block_size = FLASH_BLOCK_SIZE,
    .block_count = FLASH_SIZE / FLASH_BLOCK_SIZE,
    .block_cycles = 1000,
    .cache_size = FLASH_BLOCK_SIZE,
    .lookahead_size = 16,
    .compact_thresh = (lfs_size_t)-1,
    .read_buffer = nullptr,
    .prog_buffer = nullptr,
    .lookahead_buffer = nullptr,
    .name_max = 0,
    .file_max = 0,
    .attr_max = 0,
    .metadata_max = 0,
    .inline_max = 0,
};

bool Filesystem::init() {
  int rc = lfs_mount(&lfs, &w25q64_lfs_cfg);
  if (rc < 0) {
    rc = lfs_format(&lfs, &w25q64_lfs_cfg);

    if (rc < 0)
      return false;

    rc = lfs_mount(&lfs, &w25q64_lfs_cfg);
  }

  if (rc < 0)
    return false;

  return true;
}

int Filesystem::w25q64_read(const lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size) {
  uint32_t addr = static_cast<uint32_t>(block) * c->block_size + static_cast<uint32_t>(off);
  uint8_t *buf = static_cast<uint8_t *>(buffer);

  const uint8_t data[] = {0x0B, static_cast<uint8_t>(addr >> 16), static_cast<uint8_t>(addr >> 8),
                          static_cast<uint8_t>(addr)};

  SPI::cs_low();
  SPI::spi_write_buf(data, 4);
  SPI::spi_write_buf((const uint8_t[]) {0xFF}, 1);
  SPI::spi_read_buf(buf, size);
  SPI::cs_high();

  return 0;
}

int Filesystem::w25q64_prog(const lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer,
                            lfs_size_t size) {
  uint32_t addr = static_cast<uint32_t>(block) * c->block_size + static_cast<uint32_t>(off);
  const uint8_t *buf = static_cast<const uint8_t *>(buffer);

  // send Write Enable
  SPI::cs_low();
  SPI::spi_write_buf((const uint8_t[]) {0x06}, 1);
  SPI::cs_high();

  const uint8_t data[] = {0x02, static_cast<uint8_t>(addr >> 16), static_cast<uint8_t>(addr >> 8),
                          static_cast<uint8_t>(addr)};

  SPI::cs_low();
  SPI::spi_write_buf(data, 4);
  SPI::spi_write_buf(buf, size);
  SPI::cs_high();

  // wait for WIP bit (optional, but recommended)
  while (true) {
    uint8_t sts;
    SPI::cs_low();
    SPI::spi_write_buf((const uint8_t[]) {0x05}, 1);
    sts = SPI::spi_tx_rx(0xFF);
    SPI::cs_high();
    if (!(sts & 0x01))
      break;
  }

  return 0;
}

int Filesystem::w25q64_erase(const lfs_config *c, lfs_block_t block) {
  uint32_t addr = static_cast<uint32_t>(block) * c->block_size;

  // send Write Enable
  SPI::cs_low();
  SPI::spi_write_buf((const uint8_t[]) {0x06}, 1);
  SPI::cs_high();

  const uint8_t data[] = {0x20, static_cast<uint8_t>(addr >> 16), static_cast<uint8_t>(addr >> 8),
                          static_cast<uint8_t>(addr)};

  SPI::cs_low();
  SPI::spi_write_buf(data, 4);
  SPI::cs_high();

  // wait for WIP bit
  while (true) {
    uint8_t sts;
    SPI::cs_low();
    SPI::spi_write_buf((const uint8_t[]) {0x05}, 1);
    sts = SPI::spi_tx_rx(0xFF);
    SPI::cs_high();
    if (!(sts & 0x01))
      break;
  }

  return 0;
}

int Filesystem::w25q64_sync(const lfs_config *) { return 0; }