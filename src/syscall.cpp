#include <cstdint>

extern "C" void syscall(uint8_t num, void *arg0, void *arg1, void *arg2, void *arg3) {
  (void)num;
  (void)arg0;
  (void)arg1;
  (void)arg2;
  (void)arg3;

  // TODO: add stuff idk
}