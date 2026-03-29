#pragma once
#include <cstddef>
#include <cstdint>

extern uint8_t end;
extern uint8_t _stack;

namespace MM {
constexpr uint32_t STACK_RESERVED = 8192;
extern void *const HEAP_START;
extern void *const HEAP_END;
extern const uint32_t HEAP_SIZE;

struct Arena {
  uint32_t allocated;
  Arena *next;
  Arena *prev;
};

void init();

void *memset(void *ptr, uint8_t value, size_t n);
void *memcpy(void *dst, const void *src, size_t n);
void *malloc(size_t size);
void *calloc(size_t n, size_t size);
void free(void *p);
void *realloc(void *p, size_t size);
} // namespace MM