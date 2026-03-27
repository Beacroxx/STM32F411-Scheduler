#include <cstdint>

extern "C" void syscall(uint8_t num, void *arg0, void *arg1, void *arg2, void *arg3) {
  (void)num;
  (void)arg0;
  (void)arg1;
  (void)arg2;
  (void)arg3;

  // TODO: add stuff idk
}

// for future reference:
// asm volatile("mov r0, %0\n\t"
//              "mov r1, %1\n\t"
//              "mov r2, %2\n\t"
//              "mov r3, %3\n\t"
//              "svc %4"
//              :
//              : "r"(pa), "r"(pb), "r"(pc), "r"(pd), "I"(svc_num)
//              : "0", "1", "2", "3", "memory");