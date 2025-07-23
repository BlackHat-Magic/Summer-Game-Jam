#ifndef PIRATE_VECTOR
#define PIRATE_VECTOR

typedef struct {
    double x;
    double y;
} Vec2;

void vec2_add (Vec2* dst, Vec2* a, Vec2* b);
double vec2_dot (Vec2* a, Vec2* b);
void vec2_cross (int* dst, Vec2* a, Vec2* b);
void vec2_scale (Vec2* dst, Vec2* a, double scale);
double vec2_mag (Vec2* a);
void vec2_normalize (Vec2* dst, Vec2*a);

typedef struct {
    double x;
    double y;
    double z;
} Vec3;

void vec3_add (Vec3* dst, Vec3* a, Vec3* b);
double vec3_dot (Vec3* a, Vec3* b);
void vec3_cross (Vec3* dst, Vec3* a, Vec3* b);
void vec3_scale (Vec3* dst, Vec3* a, double scale);
double vec3_mag (Vec3* a);
void vec3_normalize (Vec3* dst, Vec3*a);

typedef struct {
    double w;
    double x;
    double y;
    double z;
} Vec4;

void vec4_add (Vec4* dst, Vec4* a, Vec4* b);
double vec4_dot (Vec4* a, Vec4* b);
void vec4_scale (Vec4* dst, Vec4* a, double scale);
double vec4_mag (Vec4* a);
void vec4_normalize (Vec4* dst, Vec4*a);

#endif