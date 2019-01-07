#ifndef ray_h__
#define ray_h__

#include <stdint.h>

#include <pthread.h>

#include "math.h"
#include "scene.h"


struct ray {
	vec3	origin;
	vec3	direction;
};

struct ray_caster_args {
	struct scene		scene;
	vec3*			raw;
	uint32_t*		samples;
	pthread_mutex_t*	pixel_mutex;
	int			seed;
};

void* ray_caster(void*);

#endif // ray_h__
