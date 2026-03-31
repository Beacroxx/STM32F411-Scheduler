#include "uart.hpp"

#include "memorymanager.hpp"
#include "scheduler.hpp"

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

#if COMM == 1

Util::circBuffer<UART::BUFFER_SIZE> UART::rx;
Util::circBuffer<UART::BUFFER_SIZE> UART::tx;

bool UART::echo = true;

static inline void handleRxNotEmpty(char c) {
  using namespace UART;
  if (rx.full())
    return;

  rx.append(c);
  if (!echo)
    return;

  tx.append(c);
  usart_enable_tx_interrupt(USART2);
}

static inline void handleTxEmpty() {
  using namespace UART;
  if (tx.empty()) {
    usart_disable_tx_interrupt(USART2);
    return;
  }

  usart_send(USART2, tx.pop());
}

extern "C" void usart2_isr() {
  if (usart_get_flag(USART2, USART_SR_RXNE))
    handleRxNotEmpty(usart_recv(USART2));
  if (usart_get_flag(USART2, USART_SR_TXE))
    handleTxEmpty();
}

#endif

size_t UART::recv(char *dst, size_t len) {
#if COMM == 1
  size_t read = 0;
  while (read < len) {
    while (rx.empty())
      Scheduler::yield();

    read += rx.copyOut(dst + read, len - read);
  }
  return read;
#else
  (void)dst;
  (void)len;
  return -1;
#endif
}

size_t UART::send(const char *src, size_t len) {
#if COMM == 1
  size_t sent = 0;
  while (sent < len) {
    while (tx.full())
      Scheduler::yield();

    bool startTx = tx.empty();
    sent += tx.copyIn(src + sent, len - sent);

    if (startTx)
      usart_enable_tx_interrupt(USART2);
  }
  return sent;
#else
  (void)src;
  (void)len;
  return -1;
#endif
}

void UART::init() {
#if COMM == 1
  rcc_periph_clock_enable(RCC_USART2);

  gpio_mode_setup(GPIOA, GPIO_MODE_AF, GPIO_PUPD_NONE, GPIO2 | GPIO3);
  gpio_set_af(GPIOA, GPIO_AF7, GPIO2 | GPIO3);

  usart_set_baudrate(USART2, 115200);
  usart_set_databits(USART2, 8);
  usart_set_stopbits(USART2, USART_STOPBITS_1);
  usart_set_mode(USART2, USART_MODE_TX_RX);
  usart_set_parity(USART2, USART_PARITY_NONE);
  usart_set_flow_control(USART2, USART_FLOWCONTROL_NONE);

  usart_enable(USART2);
  usart_enable_rx_interrupt(USART2);
  nvic_enable_irq(NVIC_USART2_IRQ);
#endif
}