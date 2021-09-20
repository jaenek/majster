typedef bool b;
typedef char i8;
typedef short int i16;
typedef long int i32;
typedef long long int i64;
typedef unsigned char u8;
typedef unsigned short int u16;
typedef unsigned long int u32;
typedef unsigned long long int u64;
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
