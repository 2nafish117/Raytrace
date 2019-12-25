#pragma once

#include "vec3.h"
#include "util.h"

struct Color
{
    union{
        unsigned char v[3];
        struct { unsigned char x, y, z; };
        struct { unsigned char r, g, b; };
    };
};

struct Sphere
{
    vec3 pos;
    float radius;
    Color color;
    unsigned int specular;
};

inline float dist_sqr(Sphere* s, vec3 p)
{
    return len_sqr(minus(s->pos, p));
}

inline float dist(Sphere* s, vec3 p)
{
    return len(minus(s->pos, p));
}

inline void intersect(Sphere* s, vec3 rayOrigin, vec3 rayDir, float* t1, float* t2)
{
    // assuming rayDir is already normalised
    const float k1 = 1.0f;
    const float k2 = 2 * dot(minus(rayOrigin, s->pos), rayDir);
    const float k3 = len_sqr(minus(rayOrigin, s->pos)) - s->radius * s->radius;

    const float determinant = k2 * k2 - 4 * k1 * k3;
    if(determinant < 0)
    {
        // doesnt intersect
        *t1 = utl::INF;
        *t2 = utl::INF;
        return;
    }
    
    // intersects
    *t1 = (-k2 + utl::sqrtf(determinant)) / 2 * k1;
    *t2 = (-k2 - utl::sqrtf(determinant)) / 2 * k1;

    // std::cout << "dir: " << rayDir.x << " " << rayDir.y << " " << rayDir.z << "\n";
    // std::cout << "org: " << rayOrigin.x << " " << rayOrigin.y << " " << rayOrigin.z << "\n";
    // std::cout << "ts : " << *t1 << " " << *t2 << "\n";
}

inline float intersect(Sphere* s, vec3 rayOrigin, vec3 rayDir)
{
    // assuming rayDir is already normalised
    float k1 = 1.0f;
    float k2 = 2 * dot(minus(s->pos, rayOrigin), rayDir);
    float k3 = len_sqr(minus(s->pos, rayOrigin)) - s->radius * s->radius;

    float determinant = k2 * k2 - 4 * k1 * k3;
    return determinant;
}