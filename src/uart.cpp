#include "uart.hpp"

#include "memorymanager.hpp"
#include "scheduler.hpp"

#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/usart.h>

#if COMM == 1

volatile uint8_t UART::rxBuffer[BUFFER_SIZE];
volatile uint16_t UART::rxHead = 0;
volatile uint16_t UART::rxTail = 0;

volatile uint8_t UART::txBuffer[BUFFER_SIZE];
volatile uint16_t UART::txHead = 0;
volatile uint16_t UART::txTail = 0;

extern "C" void usart2_isr() {
  using namespace UART;
  if (usart_get_flag(USART2, USART_SR_RXNE)) {
    char c = usart_recv(USART2);
    usart_send(USART2, c);

    rxBuffer[rxHead] = c;
    rxHead = (rxHead + 1) & (BUFFER_SIZE - 1);
  }

  if (usart_get_flag(USART2, USART_SR_TXE)) {
    if (txTail != txHead) {
      usart_send(USART2, txBuffer[txTail]);
      txTail = (txTail + 1) & (BUFFER_SIZE - 1);
    } else {
      usart_disable_tx_interrupt(USART2);
    }
  }
}

#endif

size_t UART::recv(char *dst, size_t len) {
#if COMM == 1
  size_t read = 0;
  while (read < len) {
    while (rxHead == rxTail)
      Scheduler::yield();

    uint16_t available = (rxHead - rxTail) & (BUFFER_SIZE - 1);
    uint16_t toRead = (len - read > available) ? available : len - read;
    MM::copyFromCirc(dst + read, (const void *)rxBuffer, BUFFER_SIZE, rxTail, toRead);
    rxTail = (rxTail + toRead) & (BUFFER_SIZE - 1);
    read += toRead;
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
    while (((txHead + 1) & (BUFFER_SIZE - 1)) == txTail)
      Scheduler::yield();

    uint16_t available = (txTail - txHead - 1) & (BUFFER_SIZE - 1);
    uint16_t toWrite = (len - sent > available) ? available : len - sent;
    bool startTx = (txHead == txTail);
    MM::copyToCirc((void *)txBuffer, BUFFER_SIZE, txHead, src + sent, toWrite);
    txHead = (txHead + toWrite) & (BUFFER_SIZE - 1);
    sent += toWrite;

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