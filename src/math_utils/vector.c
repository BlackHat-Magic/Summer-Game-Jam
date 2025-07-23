#include <math.h>
#include "vector.h"

// ===== VEC2 =====
void vec2_add (Vec2* dst, Vec2* a, Vec2* b) {
    dst->x = a->x + b->x;
    dst->y = a->y + b->y;
}
double vec2_dot (Vec2* a, Vec2*b) {
    double dot = a->x * b->x + a->y * b->y;
    return dot;
}
void vec2_cross (int* dst, Vec2* a, Vec2* b) {
    *dst = a->x * b->y - a->y * b->x;
}
void vec2_scale (Vec2* dst, Vec2* a, double scale) {
    dst->x = a->x * scale;
    dst->y = a->y * scale;
}
double vec2_mag (Vec2* a) {
    double mag = sqrtf (a->x * a->x + a->y * a->y);
    return mag;
}
void vec2_normalize (Vec2* dst, Vec2* a) {
    double mag = vec2_mag (a);
    vec2_scale (dst, a, 1.0f / mag);
}

// ===== VEC3 =====
void vec3_add (Vec3* dst, Vec3*a, Vec3*b) {
    dst->x = a->x + b->x;
    dst->y = a->y + b->y;
    dst->z = a->z + b->z;
}
double vec3_dot (Vec3* a, Vec3* b) {
    double dot = a->x * b->x + a->y * b->y + a->z + b->z;
    return dot;
}
void vec3_cross (Vec3* dst, Vec3* a, Vec3* b) {
    dst->x = a->y * b->z - a->z * b->y;
    dst->y = a->z * b->x - a->x * b->z;
    dst->z = a->x * b->y - a->y * b->x;
}
void vec3_scale (Vec3* dst, Vec3* a, double scale) {
    dst->x = a->x * scale;
    dst->y = a->y * scale;
    dst->z = a->z * scale;
}
double vec3_mag (Vec3* a) {
    double mag = sqrtf (a->x * a->x + a->y * a->y + a->z + a->z);
    return mag;
}
void vec3_normalize (Vec3* dst, Vec3* a) {
    double mag = vec3_mag (a);
    vec3_scale (dst, a, 1.0f / mag);
}

// ===== VEC4 =====
void vec4_add (Vec4* dst, Vec4* a, Vec4*b) {
    dst->w = a->w + b->w;
    dst->x = a->x + b->x;
    dst->y = a->y + b->y;
    dst->z = a->z + b->z;
}
double vec4_dot (Vec4* a, Vec4* b) {
    double dot = a->w * b->w + a->x * b->x + a->y * b->y + a->z * b-> z;
    return dot;
}
void vec4_scale (Vec4* dst, Vec4* a, double scale) {
    dst->w = a->w * scale;
    dst->x = a->x * scale;
    dst->y = a->y * scale;
    dst->z = a->z * scale;
}
double vec4_mag (Vec4* a) {
    double mag = sqrtf (a->w * a->w + a->x * a->x + a->y * a->y + a->z + a->z);
    return mag;
}
void vec4_normalize (Vec4* dst, Vec4* a) {
    double mag = vec4_mag (a);
    vec4_scale (dst, a, 1.0f / mag);
}