#pragma once

#define global static

#include <stdbool.h> // true, false

#include <stdint.h>
typedef uint8_t   u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef int8_t    i8;
typedef int16_t  i16;
typedef int32_t  i32;
typedef int64_t  i64;
typedef float    f32;
typedef double   f64;
typedef int32_t  b32;

#define Kilobytes(Expr) ((Expr) * 1024)
#define Megabytes(Expr) (Kilobytes(Expr) * 1024)
#define Gigabytes(Expr) (Megabytes(Expr) * 1024)

#define INLINE __forceinline
#include <assert.h>
#define Assert(Expr) assert(Expr)
#define InvalidCodePath Assert(!"InvalidCodePath")
