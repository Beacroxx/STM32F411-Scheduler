#include "malloc.hpp"

void *const Malloc::HEAP_START = (void *)(((uint32_t)&end + 3) & ~3);
const uint32_t Malloc::HEAP_SIZE = (uint32_t)&_stack - STACK_RESERVED - (uint32_t)HEAP_START;
void *const Malloc::HEAP_END = (Arena *)((uint32_t)HEAP_START + HEAP_SIZE);

void Malloc::init() {
  memset(HEAP_START, 0, HEAP_SIZE);
  Arena *origin = (Arena *)HEAP_START;
  origin->next = (Arena *)HEAP_END;
}

void *Malloc::memset(void *ptr, uint8_t value, size_t n) {
  uint8_t *p = (uint8_t *)ptr;
  const uint32_t word = value | (value << 8) | (value << 16) | (value << 24);

  // Align to 4 bytes
  if (n >= 4 && ((uint32_t)p & 3) != 0) {
    size_t head = 4 - ((uint32_t)p & 3);
    n -= head;
    while (head--)
      *p++ = value;
  }

  // Write words
  uint32_t *wp = (uint32_t *)p;
  while (n >= 4) {
    *wp++ = word;
    n -= 4;
  }

  // Write remaining bytes
  p = (uint8_t *)wp;
  while (n-- != 0)
    *p++ = value;

  return ptr;
}

void *Malloc::memcpy(void *dst, const void *src, size_t n) {
  uint8_t *_dst = (uint8_t *)dst;
  const uint8_t *_src = (uint8_t *)src;

  // Align to 4 bytes
  if (n >= 4 && ((uint32_t)_dst & 3) != 0) {
    size_t head = 4 - ((uint32_t)_dst & 3);
    n -= head;
    while (head--)
      *_dst++ = *_src++;
  }

  // Copy words
  uint32_t *_wdst = (uint32_t *)_dst;
  uint32_t *_wsrc = (uint32_t *)_src;
  while (n >= 4) {
    *_wdst++ = *_wsrc++;
    n -= 4;
  }

  // Copy remaining bytes
  _dst = (uint8_t *)_wdst;
  _src = (uint8_t *)_wsrc;
  while (n-- != 0)
    *_dst++ = *_src++;

  return dst;
}

void *Malloc::malloc(size_t size) {
  size = (size + 3) & ~3; // 4-byte align

  Arena *candidate = (Arena *)HEAP_START;

  // First allocation
  if (candidate->allocated == 0) {
    candidate->allocated = size;
    return (void *)((uint32_t)candidate + sizeof(Arena));
  }

  uint32_t curEnd;
  while (true) {
    curEnd = (uint32_t)candidate + sizeof(Arena) + candidate->allocated;
    // Check if space between blocks is enough
    if ((uint32_t)candidate->next - curEnd >= size)
      break;

    if (candidate->next != (Arena *)HEAP_END)
      candidate = candidate->next;
    else
      break;
  };

  if ((uint32_t)candidate->next - curEnd < size)
    return nullptr; // Out of memory

  // Insert new arena block
  Arena *newArena = (Arena *)curEnd;
  if (candidate->next != (Arena *)HEAP_END)
    candidate->next->prev = newArena;
  newArena->allocated = size;
  newArena->next = candidate->next;
  newArena->prev = candidate;
  candidate->next = newArena;

  return (void *)((uint32_t)newArena + sizeof(Arena));
}

void *Malloc::calloc(size_t n, size_t size) {
  void *p = malloc(n * size);
  if (p)
    memset(p, 0, n * size);
  return p;
}

void Malloc::free(void *p) {
  if (!p)
    return;

  Arena *arena = (Arena *)((uint32_t)p - sizeof(Arena));

  // Remove from linked list
  if (arena->prev)
    arena->prev->next = arena->next;
  if (arena->next != (Arena *)HEAP_END) {
    arena->next->prev = arena->prev;
    arena->next = nullptr;
  }

  arena->allocated = 0;
  arena->prev = nullptr;
}

void *Malloc::realloc(void *p, size_t size) {
  Arena *arena = (Arena *)((uint32_t)p - sizeof(Arena));

  // Shrink in place
  if (arena->allocated >= size) {
    arena->allocated = size;
    return p;
  }

  uint32_t headerEnd = (uint32_t)arena + sizeof(Arena) + arena->allocated;

  // Try to expand in place
  if ((uint32_t)arena->next - headerEnd >= size) {
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

// C linkage wrappers for newlib

extern "C" void *malloc(size_t size) { return Malloc::malloc(size); }
extern "C" void *_malloc_r(struct _reent *, size_t size) { return Malloc::malloc(size); }
extern "C" void free(void *ptr) { Malloc::free(ptr); }
extern "C" void _free_r(struct _reent *, void *ptr) { Malloc::free(ptr); }
extern "C" void *realloc(void *ptr, size_t size) { return Malloc::realloc(ptr, size); }
extern "C" void *_realloc_r(struct _reent *, void *ptr, size_t size) { return Malloc::realloc(ptr, size); }
extern "C" void *calloc(size_t nmemb, size_t size) { return Malloc::calloc(nmemb, size); }
extern "C" void *_calloc_r(struct _reent *, size_t nmemb, size_t size) { return Malloc::calloc(nmemb, size); }