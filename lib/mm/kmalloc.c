#include "kmalloc.h"

#include <stdint.h>

// 256 MiB maximum of heap
#define KM_BASE 0xD0000000 // inclusive
#define KM_TOP  0xE0000000 // exclusive

#define ARENA_SIZE 65536

#define ALIGN_UP(v, a)   ((v + (a - 1)) & (~(a - 1)))
#define ALIGN_DOWN(v, a) (v & ~(a - 1))

struct km_arena {
  struct km_arena* next;
  struct km_arena* prev;
  uint16_t slab_size;
  uint16_t total_slabs;
  uint16_t free_slabs;
  uint16_t pad;
  uint8_t bitmap[];
};

struct km_arenas {
  struct km_arena* first;
  size_t count;
  uint8_t bitmap[];
};

static struct km_arenas* arenas;

// fast log2
static const int tab32[32] = {0, 9,  1,  10, 13, 21, 2,  29, 11, 14, 16, 18, 22, 25, 3, 30,
                              8, 12, 20, 28, 15, 17, 24, 7,  19, 27, 23, 6,  26, 5,  4, 31};

static int log2_32(uint32_t value) {
  value |= value >> 1;
  value |= value >> 2;
  value |= value >> 4;
  value |= value >> 8;
  value |= value >> 16;
  return tab32[(uint32_t)(value * 0x07C4ACDD) >> 27];
}

// fast 2^n
#define pow_of_2(n) (1 << n)

// TODO: replace with real memset once implemented
static void* km_memset(void* ptr, int v, size_t num) {
  unsigned char* strPtr = (unsigned char*)ptr;
  while (num--) {
    *strPtr++ = (unsigned char)v;
  }
  return ptr;
}

static void bmp_set(uint8_t* bmp, size_t bit) { bmp[bit >> 3] |= (1 << (bit & 7)); }

static void bmp_clear(uint8_t* bmp, size_t bit) { bmp[bit >> 3] &= ~(1 << (bit & 7)); }

static int bmp_getbit(uint8_t* bmp, size_t bit) { return ((bmp[bit >> 3] >> (bit & 7)) & 1u); }

static int kmalloc_ready = 0;

int kmalloc_init() {
  if (kmalloc_ready) {
    return 0;
  }
  arenas = (struct km_arenas*)KM_BASE;
  arenas->first = NULL;
  arenas->count = 4096; // 4096 64 KiB arenas max
  km_memset(arenas->bitmap, 0, arenas->count >> 3);
  bmp_set(arenas->bitmap, 0); // reserve our own metadata

  kmalloc_ready = 1;

  return 0;
}

void* kmalloc(size_t size) {
  int order;
  size_t slab_count;
  struct km_arena* arena;
  struct km_arena* new_arena;

  if (!kmalloc_ready) {
    return NULL;
  }
  if (size == 0) {
    return NULL;
  }
  // max allocation size is 64 KiB
  if (size > 64 * 1024) {
    return NULL;
  }

  order = log2_32(size);
  if (size != pow_of_2(order)) {
    order++;
  }

  arena = arenas->first;

  while (arena) {
    if (arena->free_slabs == 0) {
      goto next;
    }
    if (arena->slab_size != order) {
      goto next;
    }

    // we found a matching arena with free slabs
    for (int i = 0; i < arena->total_slabs; ++i) {
      if (!bmp_getbit(arena->bitmap, i)) {
        continue;
      }

      bmp_set(arena->bitmap, i);
      return ((sizeof(struct km_arena) +
               (arena->total_slabs * pow_of_2(arena->slab_size) + (char*)arena + (i * arena->slab_size))));
    }

  next:
    arena = arena->next;
  }

  // we couldn't find an arena
  new_arena = NULL;
  for (int i = 0; i < arenas->count; ++i) {
    if (!bmp_getbit(arenas->bitmap, i)) {
      continue;
    }

    bmp_set(arenas->bitmap, i);
    new_arena = (struct km_arena*)(KM_BASE + i * ARENA_SIZE);
  }

  if (!new_arena) {
    return NULL;
  }

  new_arena->next = NULL;
  new_arena->slab_size = order;

  slab_count = ARENA_SIZE / pow_of_2(order);
  for (int i = 0; i < 2; i++) {
    size_t bitmap_size = (slab_count + 7) / 8;
    size_t usable = ARENA_SIZE - sizeof(struct km_arena) - bitmap_size;
    slab_count = usable / pow_of_2(order);
  }

  new_arena->total_slabs = slab_count;
  km_memset(new_arena->bitmap, 0, (new_arena->total_slabs >> 3));

  if (!arenas->first) {
    arenas->first = new_arena;
  } else {
    arena = arenas->first;
    while (arena) {
      if (arena->next = NULL) {
        new_arena->prev = arena;
        arena->next = new_arena;
        break;
      }
      arena = arena->next;
    }
  }

  bmp_set(new_arena->bitmap, 0);
  return ((sizeof(struct km_arena) + (new_arena->total_slabs * pow_of_2(new_arena->slab_size) + (char*)new_arena)));
}

void kfree(void* addr) {
  struct km_arena* arena;
  uintptr_t arena_off;
  size_t arena_id, slab_id;

  if (!addr) {
    return;
  }

  arena_off = (ALIGN_DOWN((uintptr_t)addr, ARENA_SIZE) - KM_BASE);
  arena_id = arena_off / ARENA_SIZE;
  if (!bmp_getbit(arenas->bitmap, arena_id)) {
    return;
  }

  arena = (struct km_arena*)(arena_off + KM_BASE);
  arena_off = (uintptr_t)addr - (uintptr_t)arena - KM_BASE;
  slab_id = (arena_off / arena->slab_size);
  if (bmp_getbit(arena->bitmap, slab_id)) {
    bmp_clear(arena->bitmap, slab_id);
  }
}