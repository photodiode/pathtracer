
#include <stdio.h>
#include <stdlib.h>

#include <pthread.h>
#include <string.h>

#include <stdint.h>
#include <stdbool.h>


#include "math.h"
#include "color.h"
#include "scene.h"
#include "ray.h"
#include "ppm.h"


int main() {

	uint32_t width = 512;
	uint32_t height = 512;

	struct material light	= {.color = VEC3(8, 3, 1), .roughness = 1, .opacity = 1, .ior = 1};
	struct material white	= {.color = VEC3(0.7, 0.7, 0.7), .roughness = 0.8, .opacity = 1, .ior = 1};

	struct material ball_1	= {.color = VEC3(0.9, 0.9, 0.9), .roughness = 0.05, .opacity = 0.05, .ior = 1.4};
	struct material ball_2	= {.color = VEC3(0.9, 0.9, 0.9), .roughness = 0.5, .opacity = 0.05, .ior = 1.4};
	struct material ball_3	= {.color = VEC3(0.9, 0.9, 0.9), .roughness = 0.95, .opacity = 0.05, .ior = 1.4};

	struct material ball_4	= {.color = VEC3(0.9, 0.9, 0.9), .roughness = 0.05, .opacity = 0.5, .ior = 1.4};
	struct material ball_light	= {.color = VEC3(6, 9, 10), .roughness = 1, .opacity = 1, .ior = 1};
	struct material ball_5	= {.color = VEC3(0.9, 0.9, 0.9), .roughness = 0.95, .opacity = 0.5, .ior = 1.4};

	struct material ball_6	= {.color = VEC3(0.9, 0.9, 0.9), .roughness = 0.05, .opacity = 0.95, .ior = 1.4};
	struct material ball_7	= {.color = VEC3(0.9, 0.9, 0.9), .roughness = 0.5, .opacity = 0.95, .ior = 1.4};
	struct material ball_8	= {.color = VEC3(0.9, 0.9, 0.9), .roughness = 0.95, .opacity = 0.95, .ior = 1.4};

	struct scene scene = (struct scene){
		.camera	= (struct camera){
			.position	= VEC3(0, 0, -5),

			.resolution	= VEC2I(width, height),
			.sensor		= VEC2(0.035, 0.035),

			.focal_length	= 0.05,
			.aperture	= 0.01
		},
		.sphere_count = 9,
		.spheres = (struct sphere[]){
			(struct sphere){.radius = 0.5, .position = VEC3(-1.1, 1.1, 0), .material = &ball_1},
			(struct sphere){.radius = 0.5, .position = VEC3(0, 1.1, 0), .material = &ball_2},
			(struct sphere){.radius = 0.5, .position = VEC3(1.1, 1.1, 0), .material = &ball_3},

			(struct sphere){.radius = 0.5, .position = VEC3(-1.1, 0, 0), .material = &ball_4},
			(struct sphere){.radius = 0.5, .position = VEC3(0, 0, 0), .material = &ball_light},
			(struct sphere){.radius = 0.5, .position = VEC3(1.1, 0, 0), .material = &ball_5},

			(struct sphere){.radius = 0.5, .position = VEC3(-1.1, -1.1, 0), .material = &ball_6},
			(struct sphere){.radius = 0.5, .position = VEC3(0, -1.1, 0), .material = &ball_7},
			(struct sphere){.radius = 0.5, .position = VEC3(1.1, -1.1, 0), .material = &ball_8},
		},
		.triangle_count = 6,
		.triangles = (struct triangle[]){
			(struct triangle){.a = VEC3(-1000, -4, 1000), .b = VEC3(1000, -4,  1000), .c = VEC3(-1000, -4, -1000), .material = &white},
			(struct triangle){.a = VEC3( 1000, -4, 1000), .b = VEC3(1000, -4, -1000), .c = VEC3(-1000, -4, -1000), .material = &white},
			(struct triangle){.a = VEC3( 1000, 4, 1000), .b = VEC3(-1000, 4,  1000), .c = VEC3(1000, 4, -1000), .material = &white},
			(struct triangle){.a = VEC3(-1000, 4, 1000), .b = VEC3(-1000, 4, -1000), .c = VEC3(1000, 4, -1000), .material = &white},
			(struct triangle){.a = VEC3(-4, 4, 10), .b = VEC3(4,  4, 10), .c = VEC3(-4, -4, 10), .material = &light},
			(struct triangle){.a = VEC3( 4, 4, 10), .b = VEC3(4, -4, 10), .c = VEC3(-4, -4, 10), .material = &light},
		},
	};

	// image buffers
	vec3*		raw	= calloc(width * height, sizeof(vec3));
	uint32_t*	samples	= calloc(width * height, sizeof(uint32_t));

	size_t		image_size = width * height * sizeof(rgb8);
	rgb8*		image = malloc(image_size);
	// ----

	int thread_num = 2;
	pthread_t threads[thread_num];
	pthread_mutex_t pixel_mutex = PTHREAD_MUTEX_INITIALIZER;

	// create ray caster threads
	for(int i = 0; i < thread_num; i++) {

		struct ray_caster_args args = {
			.scene = scene,
			.raw = raw,
			.samples = samples,
			.pixel_mutex = &pixel_mutex,
			.seed = i,
		};

		pthread_create(&threads[i], NULL, ray_caster, (void*)&args);
	}
	// ----

	bool quit = false;

	long start_time = time(NULL);
	long last_time = time(NULL);
	long time_elapsed = 0;

	while(!quit) {

		time_elapsed = time(NULL) - start_time;

		if (time(NULL) >= last_time + 4) {

			last_time = time(NULL);
			double total_samples = 0;

			// update image
			pthread_mutex_lock(&pixel_mutex);

			for(uint32_t i = 0; i < (width * height); i++) {
				image[i] = tonemap(raw[i], samples[i], 0.5);
				total_samples += samples[i];
			}

			pthread_mutex_unlock(&pixel_mutex);

			ppm_write("img.ppm", width, height, image, image_size);
			// ----

			total_samples /= (width * height);
			printf("%i sec (%f spp)\n", (int)time_elapsed, total_samples);

			if (total_samples >= 500) {
				break;
			}
		}
	}

	// kill threads

	free(raw);
	free(samples);
	free(image);

	return EXIT_SUCCESS;
}
