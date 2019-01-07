
#include <stdlib.h>
#include <math.h>

#include "ray.h"
#include "scene.h"

bool hit_sphere(struct ray ray, struct sphere sphere, double *distance) {

	vec3 tmp = vec3_sub(sphere.position, ray.origin);
	double tca = vec3_dot(tmp, ray.direction);

	if (tca < 0) {
		return false;
	}

	double d2 = vec3_dot(tmp, tmp) - tca * tca;
	double r2 = (sphere.radius * sphere.radius);

	if (d2 > r2) {
		return false;
	}

	double thc = sqrt(r2 - d2);
	double t0 = tca - thc;
	double t1 = tca + thc;

	if (t0 > t1) {
		double t = t0;
		t0 = t1;
		t1 = t;
	}

	if (t0 < 0) {
		t0 = t1;
		if (t0 < 0) {
			return false;
		}
	}

	*distance = t0 - 0.000001;

	return true;
}


bool hit_triangle(struct ray ray, struct triangle triangle, double *distance) {

	vec3 e1 = vec3_sub(triangle.b, triangle.a);
	vec3 e2 = vec3_sub(triangle.c, triangle.a);

	vec3 P = vec3_cross(ray.direction, e2);

	double det = vec3_dot(e1, P);

	if (det > -0.000001 && det < 0.000001) {
		return false;
	}

	double inv_det = 1 / det;

	vec3 T = vec3_sub(ray.origin, triangle.a);
	double u = vec3_dot(T, P) * inv_det;

	if (u < 0 || u > 1) {
		return false;
	}

	vec3 Q = vec3_cross(T, e1);
	double v = vec3_dot(ray.direction, Q) * inv_det;

	if (v < 0 || u + v  > 1) {
		return false;
	}

	double t0 = vec3_dot(e2, Q) * inv_det;

	if (t0 > 0.000001) {
		*distance = t0;
		return true;
	}

	return false;
}

bool hit_scene(struct ray ray, struct scene *scene, vec3 *hit_position, vec3 *hit_normal, double *_distance, struct material **material) {

	struct sphere *sphere = NULL;
	struct triangle *triangle = NULL;

	double distance = 0;
	double closest_distance = INFINITY;

	for(int s = 0; s < scene->sphere_count; s++) {
		if (hit_sphere(ray, scene->spheres[s], &distance) && distance < closest_distance) {

			sphere = &scene->spheres[s];
			closest_distance = distance;
		}
	}

	for(int s = 0; s < scene->triangle_count; s++) {
		if (hit_triangle(ray, scene->triangles[s], &distance) && distance < closest_distance) {

			sphere = NULL;
			triangle = &scene->triangles[s];
			closest_distance = distance;
		}
	}

	if (sphere) {

		*hit_position = vec3_add(ray.origin, vec3_mulf(ray.direction, closest_distance));
		*hit_normal = vec3_normalize(vec3_sub(*hit_position, sphere->position));
		*_distance = closest_distance;
		*material = sphere->material;

		return true;
	}

	if (triangle){

		*hit_position = vec3_add(ray.origin, vec3_mulf(ray.direction, closest_distance));
		*hit_normal = vec3_normalize(vec3_cross(vec3_sub(triangle->b, triangle->a), vec3_sub(triangle->c, triangle->a)));
		*_distance = closest_distance;
		*material = triangle->material;

		return true;
	}

	return false;
}
