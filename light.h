#include "vec3.h"

typedef void Light;

struct AmbLight
{
    float intensity;
    vec3 _padding[1];
};


struct PointLight
{
    float intensity;
    vec3 pos;
};

struct DirLight
{
    float intensity;
    vec3 dir;
};

float CalcPointLight(PointLight* pLight, unsigned int spec, vec3 point, vec3 norm, vec3 view)
{
    // assumed norm and view is normalised
    float intensity = 0.0f;

    vec3 lightDir = minus(pLight->pos, point);
    normalize(&lightDir);
    float lightDir_dot_norm = dot(lightDir, norm);
    intensity += pLight->intensity * utl::maxf(lightDir_dot_norm, 0);

    vec3 refl = reflect(lightDir, norm);
    normalize(&refl); // maybe redundant?
    float refl_dot_view = dot(refl, view);

    intensity += pLight->intensity * utl::powf(utl::maxf(refl_dot_view, 0), spec);
    return intensity;
}

float CalcDirLight(DirLight* dLight, unsigned int spec, vec3 point, vec3 norm, vec3 view)
{
    // assumed norm and view is normalised
    float intensity = 0.0f;
    vec3 lightDir = minus({0, 0, 0}, dLight->dir);
    normalize(&lightDir);
    float lightDir_dot_norm = dot(lightDir, norm);
    intensity += dLight->intensity * utl::maxf(lightDir_dot_norm, 0);

    vec3 refl = reflect(lightDir, norm);
    normalize(&refl); // maybe redundant?
    float refl_dot_view = dot(refl, view);

    intensity += dLight->intensity * utl::powf(utl::maxf(refl_dot_view, 0), spec);
    return intensity;
}
