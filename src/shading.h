#ifndef shading_h__
#define shading_h__

#include <stdbool.h>

#include "math.h"


vec3 lambert(vec3 normal);

vec3 reflect(vec3 direction, vec3 normal);
bool refract(vec3 v, vec3 n, double ior, vec3 *refracted);

double schlick(double cosine, double refraction_index);

#endif // shading_h__
