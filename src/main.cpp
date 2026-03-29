// STM32F411CE BlackPill V2.0 With EEPROM

#include "filesystem.hpp"
#include "malloc.hpp"
#include "math3d.hpp"
#include "scheduler.hpp"
#include "sh1106.hpp"
#include "spi.hpp"
#include "systick.hpp"
#include "trig.hpp"
#include "usb.hpp"

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>

void test() {
  gpio_toggle(GPIOC, GPIO13);
  SysTick::delayMs(1000);
  gpio_toggle(GPIOC, GPIO13);
  SysTick::delayMs(1000);
}

void test2() {
  while (1) {
    gpio_toggle(GPIOC, GPIO13);
    SysTick::delayMs(500);
  }
}

// clang-format off

constexpr i16f16_t RADIUS = i16f16_t(15);

// Vertices of a centered icosahedron with configurable radius
constexpr Vec3 vtx[12] = {
  Vec3(-RADIUS,  F_CONST::PHI * RADIUS, 0),
  Vec3( RADIUS,  F_CONST::PHI * RADIUS, 0),
  Vec3(-RADIUS, -F_CONST::PHI * RADIUS, 0),
  Vec3( RADIUS, -F_CONST::PHI * RADIUS, 0),
  Vec3(0, -RADIUS,  F_CONST::PHI * RADIUS),
  Vec3(0,  RADIUS,  F_CONST::PHI * RADIUS),
  Vec3(0, -RADIUS, -F_CONST::PHI * RADIUS),
  Vec3(0,  RADIUS, -F_CONST::PHI * RADIUS),
  Vec3( F_CONST::PHI * RADIUS, 0, -RADIUS),
  Vec3( F_CONST::PHI * RADIUS, 0,  RADIUS),
  Vec3(-F_CONST::PHI * RADIUS, 0, -RADIUS),
  Vec3(-F_CONST::PHI * RADIUS, 0,  RADIUS)
};

// Icosahedron faces (counter-clockwise winding)
const Tri3 icosahedron[] = {
  Tri3(vtx[0], vtx[11], vtx[5]),
  Tri3(vtx[0], vtx[5], vtx[1]),
  Tri3(vtx[0], vtx[1], vtx[7]),
  Tri3(vtx[0], vtx[7], vtx[10]),
  Tri3(vtx[0], vtx[10], vtx[11]),

  Tri3(vtx[1], vtx[5], vtx[9]),
  Tri3(vtx[5], vtx[11], vtx[4]),
  Tri3(vtx[11], vtx[10], vtx[2]),
  Tri3(vtx[10], vtx[7], vtx[6]),
  Tri3(vtx[7], vtx[1], vtx[8]),

  Tri3(vtx[3], vtx[9], vtx[4]),
  Tri3(vtx[3], vtx[4], vtx[2]),
  Tri3(vtx[3], vtx[2], vtx[6]),
  Tri3(vtx[3], vtx[6], vtx[8]),
  Tri3(vtx[3], vtx[8], vtx[9]),

  Tri3(vtx[4], vtx[9], vtx[5]),
  Tri3(vtx[2], vtx[4], vtx[11]),
  Tri3(vtx[6], vtx[2], vtx[10]),
  Tri3(vtx[8], vtx[6], vtx[7]),
  Tri3(vtx[9], vtx[8], vtx[1])
};

// clang-format on

void test3() {
  SH1106::init();
  SysTick::delayMs(1000);

  i16f16_t angle = 0;
  i16f16_t angle2 = 0;

  Tri2 tri2D;
  Tri3 tri3D;
  Vec3 sin;
  Vec3 cos;

  constexpr i16f16_t INCR1 = i16f16_t(0.01f);
  constexpr i16f16_t INCR2 = F_CONST::PI / 1000;

  while (1) {
    uint64_t start = SysTick::ms;
    SH1106::clear();

    Trig::sin(Vec3(angle, angle2, 0), sin);
    Trig::cos(Vec3(angle, angle2, 0), cos);

    for (uint32_t i = 0; i < sizeof(icosahedron) / sizeof(Tri3); i++) {
      Malloc::memcpy(&tri3D, &icosahedron[i], sizeof(Tri3));
      Math3D::rotate(tri3D, sin, cos);
      if (Math3D::project(tri3D, tri2D)) {
        if (tri2D.area() < 0)
          SH1106::drawTriangle(tri2D);
      }
    }

    angle += INCR1;
    angle2 += INCR2;

    SH1106::update();
    SysTick::delayMs(16, start);
  }
}

int main() {
  // Setup system clock
  rcc_clock_setup_pll(&rcc_hse_25mhz_3v3[RCC_CLOCK_3V3_96MHZ]);

  // Set SVC to prio 1 to allow long running syscalls
  nvic_set_priority(NVIC_SV_CALL_IRQ, 1);

  // Setup systick
  SysTick::init();

  // Enable GPIO clocks
  rcc_periph_clock_enable(RCC_GPIOA);
  rcc_periph_clock_enable(RCC_GPIOB);
  rcc_periph_clock_enable(RCC_GPIOC);

  // Set PC13 as output (push-pull)
  gpio_mode_setup(GPIOC, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13);
  gpio_set_output_options(GPIOC, GPIO_OTYPE_PP, GPIO_OSPEED_2MHZ, GPIO13);

  // Setup malloc
  Malloc::init();

  // Init USB
  USB::init();

  // Init SPI
  SPI::init();

  // Read JEDEC
  uint8_t id[3];
  SPI::cs_low();
  SPI::spi_write_buf((const uint8_t[]) {0x9F}, 1);
  SPI::spi_read_buf(id, 3);
  SPI::cs_high();

  // Check JEDEC ID for W25Q64JV (should be 0xEF 0x40 0x17)
  if (id[0] != 0xEF || id[1] != 0x40 || id[2] != 0x17) {
    printf("JEDEC ID did not match, expected 0xEF4017, got %02X%02X%02X\n", id[0], id[1], id[2]);
    hard_fault_handler();
  }

  // Init FS
  if (!Filesystem::init()) {
    printf("Failed to initizalize LittleFS");
    hard_fault_handler();
  }

  // Spawn Tasks
  Scheduler::createTask(test);
  Scheduler::createTask(test2);
  Scheduler::createTask(test3);

  Scheduler::start();
}
