#ifndef scene_h__
#define scene_h__

#include <stdbool.h>

#include "math.h"


struct camera {
	vec3		position;

	vec2i		resolution;
	vec2		sensor;

	double		focal_length;
	double		aperture;
};

struct material {
	vec3		color;
	double		roughness;
	double		opacity;
	double		ior;
};

struct sphere {
	vec3			position;
	double			radius;

	struct material*	material;
};

struct triangle {
	vec3			a, b, c;

	struct material*	material;
};


struct scene {
	struct camera		camera;

	int			sphere_count;
	struct sphere*		spheres;

	int			triangle_count;
	struct triangle*	triangles;
};


#endif // scene_h__
