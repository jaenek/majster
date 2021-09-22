#ifndef _TYPES_H
#define _TYPES_H

#include <stdbool.h>
#include <stdint.h>
typedef bool b;
typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;
typedef float f32;
typedef double f64;
typedef long double f128;

#include "HandmadeMath.h"
typedef hmm_v2 v2;
typedef hmm_v3 v3;
typedef hmm_v4 v4;
typedef hmm_m4 m4;
typedef hmm_quaternion quat;
#define V2(...) HMM_Vec2(__VA_ARGS__)
#define V3(...) HMM_Vec3(__VA_ARGS__)
#define V4(...) HMM_Vec4(__VA_ARGS__)
#define M4(...) HMM_Mat4(__VA_ARGS__)
#define QUAT(...) HMM_Quaternion(__VA_ARGS__)

#endif // _TYPES_H
