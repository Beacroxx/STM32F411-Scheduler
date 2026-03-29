#pragma once
#include <cstddef>
#include <cstdint>

extern uint8_t end;
extern uint8_t _stack;

namespace MM {

/**
 * @brief Number of bytes reserved for the stack.
 */
constexpr uint32_t STACK_RESERVED = 8192;

/**
 * @brief Start address of the heap, aligned up to 4 bytes.
 */
extern void *const HEAP_START;

/**
 * @brief End address of the heap.
 */
extern void *const HEAP_END;

/**
 * @brief Total size of the heap in bytes.
 */
extern const uint32_t HEAP_SIZE;

/**
 * @brief Arena structure for memory management.
 */
struct Arena {
  uint32_t allocated; ///< Number of bytes allocated in this arena
  Arena *next;        ///< Pointer to next arena
  Arena *prev;        ///< Pointer to previous arena
};

/**
 * @brief Initialize the memory manager and heap.
 */
void init();

/**
 * @brief Set a block of memory to a value.
 *
 * Optimized for 4-byte aligned quad word fill on Cortex-M4.
 *
 * @param ptr Pointer to memory block.
 * @param value Value to set.
 * @param n Number of bytes.
 * @return Pointer to memory block.
 */
void *memset(void *ptr, uint8_t value, size_t n);

/**
 * @brief Copy memory from source to destination.
 *
 * Optimized for 4-byte aligned quad word copy on Cortex-M4.
 *
 * @param dst Destination pointer.
 * @param src Source pointer.
 * @param n Number of bytes.
 * @return Pointer to destination.
 */
void *memcpy(void *dst, const void *src, size_t n);

/**
 * @brief Allocate memory block.
 * @param size Number of bytes to allocate.
 * @return Pointer to allocated memory or nullptr on failure.
 */
void *malloc(size_t size);

/**
 * @brief Allocate zero-initialized memory for an array.
 * @param n Number of elements.
 * @param size Size of each element.
 * @return Pointer to allocated memory or nullptr on failure.
 */
void *calloc(size_t n, size_t size);

/**
 * @brief Free previously allocated memory.
 * @param p Pointer to memory to free.
 */
void free(void *p);

/**
 * @brief Reallocate memory block to new size.
 * @param p Pointer to previously allocated memory.
 * @param size New size in bytes.
 * @return Pointer to reallocated memory or nullptr on failure.
 */
void *realloc(void *p, size_t size);
} // namespace MM