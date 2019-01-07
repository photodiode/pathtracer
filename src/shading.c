
#include <math.h>

#include "shading.h"

vec3 transform_to_world(double x, double y, double z, vec3 normal) {

	vec3 main_axis;
	if (fabs(normal.x) < 0.57735026919) {
		main_axis = VEC3(1, 0, 0);
	} else if (fabs(normal.y) < 0.57735026919) {
		main_axis = VEC3(0, 1, 0);
	} else {
		main_axis = VEC3(0, 0, 1);
	}

	vec3 u = vec3_normalize(vec3_cross(normal, main_axis));
	vec3 v = vec3_cross(normal, u);
	vec3 w = normal;

	return vec3_add(vec3_add(vec3_mulf(u, x), vec3_mulf(v, y)), vec3_mulf(w, z));
}

vec3 lambert(vec3 normal) {
	double r = sqrt(random()) * 1;
	double theta = random() * 2.0 * PI;

	double x = r * cos(theta);
	double y = r * sin(theta);

	// Project z up to the unit hemisphere
	double z = sqrt(1.0 - x * x - y * y);

	return vec3_normalize(transform_to_world(x, y, z, normal));
}

vec3 reflect(vec3 direction, vec3 normal) {
	return vec3_sub(direction, vec3_mulf(normal, 2 * vec3_dot(normal, direction)));
}

bool refract(vec3 v, vec3 n, double ior, vec3 *refracted) {

	vec3 uv = vec3_normalize(v);
	double dt = vec3_dot(uv, n);
	double discriminant = 1.0 - ior*ior*(1-dt*dt);

	if (discriminant > 0) {
		*refracted = vec3_sub(vec3_mulf(vec3_sub(uv, vec3_mulf(n, dt)), ior), vec3_mulf(n, sqrt(discriminant)));
		return true;
	}

	return false;
}

double schlick(double cosine, double refraction_index) {
	double r0 = (1 - refraction_index) / (1 + refraction_index);
	r0 = r0*r0;
	return r0 + (1 - r0) * pow((1 - cosine), 5);
}
