#include "printf.hpp"

#include "memorymanager.hpp"
#include "types.hpp"
#include "uart.hpp"
#include "usb.hpp"

#include <cstring>

template <typename T> uint32_t digitCount(T v) {
  using U = Util::makeUnsignedT<T>;
  if (v == 0)
    return 1;

  U value = v;

  int count = 0;
  if constexpr (Util::isSignedV<T>) {
    if (v < 0) {
      if (v == (T(1)) << (sizeof(T) * 8 - 1)) {
        value = U(-(v + 1)) + 1;
      } else {
        value = -v;
        count = 1;
      }
    }
  }

  while (value) {
    value /= 10;
    count++;
  }
  return count;
}

template <> uint32_t digitCount(i16f16_t v) {
  int32_t ip = static_cast<int32_t>(v);

  int count = 0;
  if (ip < 0) {
    count = 1;
    ip = -ip;
  } else if (ip == 0) {
    count = 1;
  } else {
    while (ip) {
      ip /= 10;
      count++;
    }
  }

  count += 5; // '.' + 4 decimals
  return count;
}

template <typename T> char *hexToBuf(char *buf, T v) {
  uint32_t nibbles = sizeof(T) * 2;
  for (int32_t i = nibbles - 1; i >= 0; i--) {
    uint8_t nibble = (v >> (i * 4)) & 0xF;
    *buf++ = nibble < 10 ? '0' + nibble : 'A' + nibble - 10;
  }
  return buf;
}

template <> char *hexToBuf(char *buf, i16f16_t v) {
  uint32_t nibbles = sizeof(i16f16_t) * 2;
  for (int32_t i = nibbles - 1; i >= 0; i--) {
    uint8_t nibble = (v.value >> (i * 4)) & 0xF;
    *buf++ = nibble < 10 ? '0' + nibble : 'A' + nibble - 10;
  }
  return buf;
}

template <typename T> char *toBuf(char *buf, T v) {
  using U = Util::makeUnsignedT<T>;
  if (v == 0) {
    *buf++ = '0';
    return buf;
  }

  U value = v;

  if constexpr (Util::isSignedV<T>) {
    if (v < 0) {
      *buf++ = '-';
      if (v == (T(1)) << (sizeof(T) * 8 - 1)) {
        value = U(-(v + 1)) + 1;
      } else {
        value = -v;
      }
    }
  }

  uint32_t i = 0;
  while (value) {
    buf[i++] = '0' + (value % 10);
    value /= 10;
  }

  uint32_t l = 0;
  uint32_t r = i - 1;
  while (l < r) {
    char tmp = buf[l];
    buf[l++] = buf[r];
    buf[r--] = tmp;
  }

  return buf + i;
}

template <> char *toBuf(char *buf, i16f16_t v) {
  int32_t ip = static_cast<int32_t>(v);
  buf = toBuf(buf, ip);
  *buf++ = '.';
  uint32_t frac = (v.value & 0xFFFF) * 10000 / 65536;
  for (uint32_t i = 1000; i; i /= 10)
    *buf++ = '0' + (frac / i) % 10;
  return buf;
}

template <> char *toBuf(char *buf, const char *s) {
  size_t len = strlen(s);
  MM::memcpy(buf, s, len);
  return buf + len;
}

int __sprintfImpl(const Util::fmtStr &fmt, char *dst, Util::fmtArg *args, size_t n) {
  char *orig = dst;
  size_t argIdx = 0;
  char modifier = '\0';
  for (size_t i = 0; i < fmt.size; i++) {
    if (i + 2 >= fmt.size || fmt[i] != '{' || (fmt[i + 1] != '}' && fmt[i + 2] != '}')) {
      *dst++ = fmt[i];
      continue;
    }

    i++;

    if (fmt[i] != '}') {
      modifier = fmt[i];
      i++;
    }

    if (argIdx >= n)
      continue;

    const Util::fmtArg &arg = args[argIdx++];
    switch (arg.type) {
    case Util::fmtType::INT8: {
      int8_t v = *static_cast<const int8_t *>(arg.ptr);
      dst = modifier == 'X' ? hexToBuf(dst, v) : toBuf(dst, v);
      break;
    }
    case Util::fmtType::UINT8: {
      uint8_t v = *static_cast<const uint8_t *>(arg.ptr);
      dst = modifier == 'X' ? hexToBuf(dst, v) : toBuf(dst, v);
      break;
    }
    case Util::fmtType::INT16: {
      int16_t v = *static_cast<const int16_t *>(arg.ptr);
      dst = modifier == 'X' ? hexToBuf(dst, v) : toBuf(dst, v);
      break;
    }
    case Util::fmtType::UINT16: {
      uint16_t v = *static_cast<const uint16_t *>(arg.ptr);
      dst = modifier == 'X' ? hexToBuf(dst, v) : toBuf(dst, v);
      break;
    }
    case Util::fmtType::INT32: {
      int32_t v = *static_cast<const int32_t *>(arg.ptr);
      dst = modifier == 'X' ? hexToBuf(dst, v) : toBuf(dst, v);
      break;
    }
    case Util::fmtType::UINT32: {
      uint32_t v = *static_cast<const uint32_t *>(arg.ptr);
      dst = modifier == 'X' ? hexToBuf(dst, v) : toBuf(dst, v);
      break;
    }
    case Util::fmtType::INT64: {
      int64_t v = *static_cast<const int64_t *>(arg.ptr);
      dst = modifier == 'X' ? hexToBuf(dst, v) : toBuf(dst, v);
      break;
    }
    case Util::fmtType::UINT64: {
      uint64_t v = *static_cast<const uint64_t *>(arg.ptr);
      dst = modifier == 'X' ? hexToBuf(dst, v) : toBuf(dst, v);
      break;
    }
    case Util::fmtType::I16F16: {
      i16f16_t v = *static_cast<const i16f16_t *>(arg.ptr);
      dst = modifier == 'X' ? hexToBuf(dst, v) : toBuf(dst, v);
      break;
    }
    case Util::fmtType::CSTR: {
      const char *s = static_cast<const char *>(arg.ptr);
      dst = toBuf(dst, s);
      break;
    }

    case Util::fmtType::NONE:
      break;
    }
  }

  *dst = '\0';

  return dst - orig;
}

int __printfImpl(const Util::fmtStr &fmt, Util::fmtArg *args, size_t n) {
  size_t total = 0;
  size_t argIdx = 0;
  char modifier = '\0';
  for (size_t i = 0; i < fmt.size; i++) {
    if (i + 2 >= fmt.size || fmt[i] != '{' || (fmt[i + 1] != '}' && fmt[i + 2] != '}')) {
      total++;
      continue;
    }

    i++;

    if (fmt[i] != '}') {
      modifier = fmt[i];
      i++;
    }

    if (argIdx >= n)
      continue;

    const Util::fmtArg &arg = args[argIdx++];
    switch (arg.type) {
    case Util::fmtType::INT8:
      total += modifier == 'X' ? sizeof(int8_t) * 2 : digitCount(*static_cast<const int8_t *>(arg.ptr));
      break;
    case Util::fmtType::UINT8:
      total += modifier == 'X' ? sizeof(uint8_t) * 2 : digitCount(*static_cast<const uint8_t *>(arg.ptr));
      break;
    case Util::fmtType::INT16:
      total += modifier == 'X' ? sizeof(int16_t) * 2 : digitCount(*static_cast<const int16_t *>(arg.ptr));
      break;
    case Util::fmtType::UINT16:
      total += modifier == 'X' ? sizeof(uint16_t) * 2 : digitCount(*static_cast<const uint16_t *>(arg.ptr));
      break;
    case Util::fmtType::INT32:
      total += modifier == 'X' ? sizeof(int32_t) * 2 : digitCount(*static_cast<const int32_t *>(arg.ptr));
      break;
    case Util::fmtType::UINT32:
      total += modifier == 'X' ? sizeof(uint32_t) * 2 : digitCount(*static_cast<const uint32_t *>(arg.ptr));
      break;
    case Util::fmtType::INT64:
      total += modifier == 'X' ? sizeof(int64_t) * 2 : digitCount(*static_cast<const int64_t *>(arg.ptr));
      break;
    case Util::fmtType::UINT64:
      total += modifier == 'X' ? sizeof(uint64_t) * 2 : digitCount(*static_cast<const uint64_t *>(arg.ptr));
      break;
    case Util::fmtType::I16F16:
      total += modifier == 'X' ? sizeof(i16f16_t) * 2 : digitCount(*static_cast<const i16f16_t *>(arg.ptr));
      break;
    case Util::fmtType::CSTR:
      total += strlen(static_cast<const char *>(arg.ptr));
      break;
    case Util::fmtType::NONE:
      break;
    }
  }

  total++;

  char *buf = static_cast<char *>(MM::malloc(total));
  if (!buf)
    return -1;

  size_t len = __sprintfImpl(fmt, buf, args, n);

#if COMM == 2
  USB::send(buf, len);
#elif COMM == 1
  UART::send(buf, len);
#endif

  MM::free(buf);

  return len;
}
