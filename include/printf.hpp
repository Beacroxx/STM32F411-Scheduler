#pragma once

#include "types.hpp"

int __printfImpl(const Util::FmtStr &fmt, Util::fmtArg *args, size_t n);
int __sprintfImpl(const Util::FmtStr &fmt, char *dst, Util::fmtArg *args, size_t n);

template <typename... Args> int printf(const Util::FmtStr &fmt, const Args &...args) {
  Util::fmtArg argStore[] = {
      Util::fmtArg {Util::argTypeV<Args>, &args}
       ...
  };

  return __printfImpl(fmt, argStore, sizeof...(args));
}

template <typename... Args> int sprintf(const Util::FmtStr &fmt, char *dst, const Args &...args) {
  Util::fmtArg argStore[] = {
      Util::fmtArg {Util::argTypeV<Args>, &args}
       ...
  };

  return __sprintfImpl(fmt, dst, argStore, sizeof...(args));
}
