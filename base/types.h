#ifndef H_TYPES
#define H_TYPES

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef float f32;
typedef double f64;

#if SIZE_MAX == UINT64_MAX
#define SIZE_T_IS_64_BIT
#elif SIZE_MAX == UINT32_MAX
#define SIZE_T_IS_32_BIT
#else
#error Non standard size_t width
#endif

#ifdef SIZE_T_IS_64_BIT
typedef u64 usize;
typedef i64 isize;
#else
typedef u32 usize;
typedef i32 isize;
#endif

#define REINTERPRET_CAST(type, value) *(type *)&value

#endif