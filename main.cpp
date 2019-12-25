#include "qdbmp.h"
#include "vec3.h"
#include "util.h"
#include "Sphere.h"
#include "Light.h"

typedef unsigned int u32;
typedef int s32;

const u32 cWidth = 1920;
const u32 cHeight = 1080;

const float aspectRatio = (float) cWidth / cHeight;

const float vWidth = aspectRatio * 1;
const float vHeight = 1;
const float vDist = 1;

const float T_MIN = 0.1f;
const float T_MAX = 100.0f;

const Color BACKGROUND_COLOR = {25, 25, 25};

struct Scene
{
    //Sphere sphere;
    u32 numSpheres;
    Sphere spheres[3];

    AmbLight ambLight;
    PointLight pointLight;
    DirLight dirLight;
};

void ConstructScene(Scene* scene)
{
    scene->numSpheres = 3;
    scene->spheres[0] = { {0, 0, 5}, 1, {50, 230, 100}, 20 };
    scene->spheres[1] = { {1, 1, 5}, 1, {230, 50, 100}, 8 };
    scene->spheres[2] = { {-1, 1, 5}, 1, {50, 100, 230}, 10 };

    scene->ambLight.intensity = 0.1f;

    scene->pointLight.intensity = 0.5f;
    scene->pointLight.pos = { 2, 1, 0 };

    scene->dirLight.intensity = 0.2f;
    scene->dirLight.dir = { 1, 4, 4 };
    normalize(&scene->dirLight.dir);
}

inline vec3 CanvToViewp(u32 px, u32 py)
{
    return {
        ((float)px / cWidth - 0.5f) * vWidth,
        -((float)py / cHeight - 0.5f) * vHeight,
        vDist
    };
}

float CalcLight(Scene* scene, vec3 point, vec3 norm, u32 spec)
{
    // norm is assumed normalised

    // ambient light
    float intensity = 0.0f;
    intensity += scene->ambLight.intensity;

    vec3 view = minus({0, 0, 0}, point);
    normalize(&view);

    intensity += CalcPointLight(&scene->pointLight, spec, point, norm, view);
    intensity += CalcDirLight(&scene->dirLight, spec, point, norm, view);

    return intensity;
}

Color RayTrace(Scene* scene, vec3 rayOrigin, vec3 rayDir, float t_min, float t_max)
{
    // rayDir is assumed normalised
    float closest_t = utl::INF;
    Sphere* closest_sphere = NULL;
    
    for(u32 i = 0;i < scene->numSpheres;++i)
    {
        float t1, t2;
        intersect(&scene->spheres[i], rayOrigin, rayDir, &t1, &t2);

        if(utl::in_range(t1, t_min, t_max) && t1 < closest_t)
        {
            closest_t = t1;
            closest_sphere = &scene->spheres[i];
        }
        if(utl::in_range(t2, t_min, t_max) && t2 < closest_t)
        {
            closest_t = t2;
            closest_sphere = &scene->spheres[i];
        }
    }

    if(closest_sphere == NULL)
        return BACKGROUND_COLOR;
    
    vec3 point = plus(rayOrigin, scalar_prod(rayDir, closest_t));
    vec3 norm = minus(point, closest_sphere->pos);
    normalize(&norm);

    Color result = closest_sphere->color;
    float lightIntensity = CalcLight(scene, point, norm, closest_sphere->specular);
    lightIntensity = utl::clampf01(lightIntensity);
    result.r *= lightIntensity;
    result.g *= lightIntensity;
    result.b *= lightIntensity;

    return result;
}

int main()
{
    BMP* image = BMP_Create(cWidth, cHeight, 32);

    Scene scene;
    ConstructScene(&scene);

    float t_min = 0.1f;
    float t_max = 100.0f;

    for(u32 y = 0;y < cHeight;++y)
    {
        for(u32 x = 0;x < cWidth;++x)
        {
            vec3 vPos = CanvToViewp(x, y);
            // Camera pos
            vec3 rayOrigin = {0, 0, 0};
            vec3 rayDir = minus(vPos, rayOrigin);
            normalize(&rayDir);

            Color pColor = RayTrace(&scene, rayOrigin, rayDir, t_min, t_max);

            BMP_SetPixelRGB(image, x, y, pColor.r, pColor.g, pColor.b);
        }
    }

    BMP_WriteFile(image, "image.bmp");
    BMP_Free(image);
    return 0;
}