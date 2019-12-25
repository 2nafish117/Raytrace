#pragma once

//#include <cassert>
//#include <cmath>
#include "util.h"

struct vec3 
{
    union{
        float v[3];
        struct { float x, y, z; };
    };
};

inline vec3 plus(vec3 v1, vec3 v2);
inline vec3 minus(vec3 v1, vec3 v2);
inline vec3 scalar_prod(vec3 v, float scalar);
inline float dot(vec3 v1, vec3 v2);

inline vec3 unit(vec3 v);
inline void normalize(vec3* v);
inline float len_sqr(vec3 v);
inline float len(vec3 v);

inline vec3 reflect(vec3 lightDir, vec3 norm)
{
    //R = 2*N*dot(N, L) - L
    // lightDir and norm are assumed to be normalised
    // vec3 reflectedAlongNorm = scalar_prod(norm, dot(lightDir, norm));
    // vec3 reflectedAlongMirr = minus(reflectedAlongNorm, lightDir);
    // return plus(reflectedAlongNorm, reflectedAlongMirr);

    return minus(scalar_prod(norm, 2 * dot(norm, lightDir)), lightDir);
}

inline vec3 plus(vec3 v1, vec3 v2)
{
    return {
        v1.x + v2.x,
        v1.y + v2.y,
        v1.z + v2.z,
    };
}

inline vec3 minus(vec3 v1, vec3 v2)
{
    return {
        v1.x - v2.x,
        v1.y - v2.y,
        v1.z - v2.z,
    };
}

inline vec3 scalar_prod(vec3 v, float scalar)
{
    return {
        v.x * scalar,
        v.y * scalar,
        v.z * scalar
    };
}

inline float dot(vec3 v1, vec3 v2)
{
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
}

inline vec3 unit(vec3 v)
{
    float l = len(v);
    //assert(l != 0.0f && "vector of 0 len");
    return {
        v.x / l,
        v.y / l,
        v.z / l
    };
}

inline void normalize(vec3* v)
{
    float l = len(*v);
    //assert(l != 0.0f && "vector of 0 len");

    v->x = v->x / l;
    v->y = v->y / l;
    v->z = v->z / l;
}

inline float len_sqr(vec3 v)
{
    return dot(v, v);
}

inline float len(vec3 v)
{
    return utl::sqrtf(dot(v, v));
}


