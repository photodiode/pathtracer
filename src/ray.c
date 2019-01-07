
#include <stdlib.h>
#include <stdio.h>

#include <math.h>

#include "halton.h"
#include "shading.h"
#include "ray.h"


bool hit_scene(struct ray ray, struct scene *scene, vec3 *hit_position, vec3 *hit_normal, double *_distance, struct material **material);


vec3 trace(struct ray ray, struct scene *scene, int max_bounces) {

	vec3 color = VEC3(1, 1, 1);

	struct material *material;
	vec3 hit_normal, hit_position;
	double distance;

	for(int bounce = max_bounces; bounce--;) {

		if (!hit_scene(ray, scene, &hit_position, &hit_normal, &distance, &material)) {
			color = vec3_mul(color, VEC3(0, 0, 0));
			break;
		}

		// if the material is an emitter
		if (material->color.r > 1 || material->color.g > 1 || material->color.b > 1) {
			color = vec3_mul(color, material->color);
			break;
		}

		color = vec3_mul(color, material->color);

		double reflect_probability = 1;
		vec3 refract_direction;

		if (material->opacity < 1) {

			vec3 outward_normal;
			double ior, cosine;

			if (vec3_dot(ray.direction, hit_normal) > 0) {
				outward_normal = VEC3(-hit_normal.x, -hit_normal.y, -hit_normal.z);
				ior = material->ior;
				cosine = material->ior * vec3_dot(ray.direction, hit_normal);
			}else{
				outward_normal = hit_normal;
				ior = 1.0 / material->ior;
				cosine = -vec3_dot(ray.direction, hit_normal);
			}

			if (random() > material->opacity) {

				outward_normal = vec3_normalize(
					vec3_add(
						vec3_mulf(outward_normal, 1 - material->roughness),
						vec3_mulf(lambert(outward_normal), material->roughness)
					)
				);

				if (refract(ray.direction, outward_normal, ior, &refract_direction)) {
					reflect_probability = schlick(cosine,  material->ior);
				}
			}
		}

		vec3 new_position;
		vec3 new_direction;

		if (random() < reflect_probability) {
			new_position = hit_position;
			new_direction = vec3_normalize(
				vec3_add(
					vec3_mulf(reflect(ray.direction, hit_normal), 1 - material->roughness),
					vec3_mulf(lambert(hit_normal), material->roughness)
				)
			);

		}else{ // refract
			new_position = vec3_add(ray.origin, vec3_mulf(ray.direction, distance+0.000002)); // jump across surface
			new_direction = refract_direction;
		}

		ray.origin = new_position;
		ray.direction = new_direction;
	}

	return color;
}

struct ray make_ray(struct camera *camera, double x, double y, double ha, double hb) {

	double rt = camera->aperture * sqrt(ha);
	double theta = hb * 2 * PI;

	double rx = rt * cos(theta);
	double ry = rt * sin(theta);

	rx -= camera->aperture * 0.5;
	ry -= camera->aperture * 0.5;

	return (struct ray){
		.origin = VEC3(
			camera->position.x + rx,
			camera->position.y + ry,
			camera->position.z
		),
		.direction = vec3_normalize(VEC3(
			 (x - 0.5) * camera->sensor.w,
			-(y - 0.5) * camera->sensor.h,
			camera->focal_length
		))
	};
}

void* ray_caster(void *arg_pointer) {

	struct ray_caster_args *args = (struct ray_caster_args *)arg_pointer;

	double hx, hy;
	int x, y;
	vec3 pixel;

	// bases for sensor pixel
	halton_state hsx = halton_new(args->seed);
	halton_state hsy = halton_new((args->seed+1)*3);

	// bases for aperture
	halton_state hsax = halton_new((args->seed+1)*7);
	halton_state hsay = halton_new((args->seed+1)*11);

	while(1) {
		hx = halton_get(&hsx);
		hy = halton_get(&hsy);

		pixel = trace(
			make_ray(&args->scene.camera, hx, hy, halton_get(&hsax), halton_get(&hsay)),
			&args->scene,
			16
		);

		x = hx * (double)args->scene.camera.resolution.w;
		y = hy * (double)args->scene.camera.resolution.h;

		// update pixel
		pthread_mutex_lock(args->pixel_mutex);

		args->raw[x + y * args->scene.camera.resolution.w].r += pixel.r;
		args->raw[x + y * args->scene.camera.resolution.w].g += pixel.g;
		args->raw[x + y * args->scene.camera.resolution.w].b += pixel.b;

		args->samples[x + y * args->scene.camera.resolution.w] += 1;

		pthread_mutex_unlock(args->pixel_mutex);
		// ----
	}

	return NULL;
}
