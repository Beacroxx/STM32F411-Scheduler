#include "memorymanager.hpp"

#include <libopencm3/cm3/cortex.h>

void *const MM::HEAP_START = reinterpret_cast<void *>((reinterpret_cast<uint32_t>(&end) + 3) & ~3);
const uint32_t MM::HEAP_SIZE =
    reinterpret_cast<uint32_t>(&_stack) - STACK_RESERVED - reinterpret_cast<uint32_t>(HEAP_START);
void *const MM::HEAP_END = reinterpret_cast<Arena *>(reinterpret_cast<uint32_t>(HEAP_START) + HEAP_SIZE);

void MM::init() {
  memset(HEAP_START, 0, HEAP_SIZE);
  Arena *origin = reinterpret_cast<Arena *>(HEAP_START);
  origin->next = (Arena *)HEAP_END;
}

void *MM::malloc(size_t size) {
  cm_disable_interrupts();
  size = (size + 3) & ~3; // 4-byte align

  Arena *candidate = reinterpret_cast<Arena *>(HEAP_START);

  // First allocation
  if (candidate->allocated == 0) {
    candidate->allocated = size;
    cm_enable_interrupts();
    return reinterpret_cast<void *>(reinterpret_cast<uint32_t>(candidate) + sizeof(Arena));
  }

  uint32_t curEnd;
  while (true) {
    curEnd = reinterpret_cast<uint32_t>(candidate) + sizeof(Arena) + candidate->allocated;
    // Check if space between blocks is enough
    if (reinterpret_cast<uint32_t>(candidate->next) - curEnd >= size)
      break;

    if (candidate->next != reinterpret_cast<Arena *>(HEAP_END))
      candidate = candidate->next;
    else
      break;
  };

  if (reinterpret_cast<uint32_t>(candidate->next) - curEnd < size) {
    cm_enable_interrupts();
    return nullptr; // Out of memory
  }

  // Insert new arena block
  Arena *newArena = reinterpret_cast<Arena *>(curEnd);
  if (candidate->next != reinterpret_cast<Arena *>(HEAP_END))
    candidate->next->prev = newArena;
  newArena->allocated = size;
  newArena->next = candidate->next;
  newArena->prev = candidate;
  candidate->next = newArena;

  cm_enable_interrupts();
  return reinterpret_cast<void *>(reinterpret_cast<uint32_t>(newArena) + sizeof(Arena));
}

void *MM::calloc(size_t n, size_t size) {
  void *p = malloc(n * size);
  if (p)
    memset(p, 0, n * size);
  return p;
}

void MM::free(void *p) {
  if (!p)
    return;

  Arena *arena = reinterpret_cast<Arena *>(reinterpret_cast<uint32_t>(p) - sizeof(Arena));

  // Remove from linked list
  if (arena->prev)
    arena->prev->next = arena->next;
  if (arena->next != reinterpret_cast<Arena *>(HEAP_END)) {
    arena->next->prev = arena->prev;
    arena->next = nullptr;
  }

  arena->allocated = 0;
  arena->prev = nullptr;
}

void *MM::realloc(void *p, size_t size) {
  Arena *arena = reinterpret_cast<Arena *>(reinterpret_cast<uint32_t>(p) - sizeof(Arena));

  // Shrink in place
  if (arena->allocated >= size) {
    arena->allocated = size;
    return p;
  }

  uint32_t headerEnd = reinterpret_cast<uint32_t>(arena) + sizeof(Arena) + arena->allocated;

  // Try to expand in place
  if (reinterpret_cast<uint32_t>(arena->next) - headerEnd >= size) {
    arena->allocated = size;
    return p;
  }

  // Allocate new block and copy
  void *np = malloc(size);
  if (!np)
    return nullptr;

  memcpy(np, p, arena->allocated);
  free(p);

  return np;
}

size_t MM::copyToCirc(void *circBuf, size_t bufSize, size_t head, const void *src, size_t len) {
  size_t first = bufSize - head;
  if (len <= first) {
    MM::memcpy((uint8_t *)circBuf + head, src, len);
  } else {
    MM::memcpy((uint8_t *)circBuf + head, src, first);
    MM::memcpy(circBuf, (const uint8_t *)src + first, len - first);
  }
  return len;
}

size_t MM::copyFromCirc(void *dst, const void *circBuf, size_t bufSize, size_t tail, size_t len) {
  size_t first = bufSize - tail;
  if (len <= first) {
    MM::memcpy(dst, (const uint8_t *)circBuf + tail, len);
  } else {
    MM::memcpy(dst, (const uint8_t *)circBuf + tail, first);
    MM::memcpy((uint8_t *)dst + first, circBuf, len - first);
  }
  return len;
}

// C linkage wrappers for newlib

extern "C" void *malloc(size_t size) { return MM::malloc(size); }
extern "C" void *_malloc_r(struct _reent *, size_t size) { return MM::malloc(size); }
extern "C" void free(void *ptr) { MM::free(ptr); }
extern "C" void _free_r(struct _reent *, void *ptr) { MM::free(ptr); }
extern "C" void *realloc(void *ptr, size_t size) { return MM::realloc(ptr, size); }
extern "C" void *_realloc_r(struct _reent *, void *ptr, size_t size) { return MM::realloc(ptr, size); }
extern "C" void *calloc(size_t nmemb, size_t size) { return MM::calloc(nmemb, size); }
extern "C" void *_calloc_r(struct _reent *, size_t nmemb, size_t size) { return MM::calloc(nmemb, size); }

extern "C" void *memcpy(void *dst, const void *src, size_t n) { return MM::memcpy(dst, src, n); }
extern "C" void *memset(void *dst, int value, size_t n) { return MM::memset(dst, value, n); }