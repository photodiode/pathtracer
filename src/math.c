
#include <stdlib.h>

#include <math.h>

#include "math.h"


#define TO_RADIANS(V) (V * 0.01745329251994329) // PI / 180


double random() {
	return (double)rand() / RAND_MAX;
}

double clamp(double f, double min, double max) {
	const double t = f < min ? min : f;
	return t > max ? max : t;
}


// 3d vector
vec3 vec3_add(const vec3 v0, const vec3 v1) {
	return VEC3(
		v0.x + v1.x,
		v0.y + v1.y,
		v0.z + v1.z
	);
}

vec3 vec3_sub(const vec3 v0, const vec3 v1) {
	return VEC3(
		v0.x - v1.x,
		v0.y - v1.y,
		v0.z - v1.z
	);
}

vec3 vec3_mul(const vec3 v0, const vec3 v1) {
	return VEC3(
		v0.x * v1.x,
		v0.y * v1.y,
		v0.z * v1.z
	);
}

vec3 vec3_mulf(const vec3 v, const double f) {
	return VEC3(
		v.x * f,
		v.y * f,
		v.z * f
	);
}

vec3 vec3_divf(const vec3 v, const double f) {
	return VEC3(
		v.x / f,
		v.y / f,
		v.z / f
	);
}

double vec3_length(const vec3 v) {
	return sqrt(v.x * v.x + v.y * v.y + v.z * v.z);
}

double vec3_dist(const vec3 v0, const vec3 v1) {
	return sqrt(((v1.x - v0.x)*(v1.x - v0.x)) + ((v1.y - v0.y)*(v1.y - v0.y)) + ((v1.z - v0.z)*(v1.z - v0.z)));
}

vec3 vec3_normalize(const vec3 v) {
	double l = vec3_length(v);
	return VEC3(
		v.x / l,
		v.y / l,
		v.z / l
	);
}

vec3 vec3_cross(const vec3 v0, const vec3 v1) {
	return VEC3(
		v0.y * v1.z - v0.z * v1.y,
		v0.z * v1.x - v0.x * v1.z,
		v0.x * v1.y - v0.y * v1.x
	);
}

double vec3_dot(const vec3 v0, const vec3 v1) {
	return v0.x * v1.x + v0.y * v1.y + v0.z * v1.z;
}
// ----
