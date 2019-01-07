#ifndef math_h__
#define math_h__

#define PI 3.14159265359

// vector 2 int
typedef union {
	struct {
		int x, y;
	};
	struct {
		int w, h;
	};
	int raw[2];
} vec2i;

// vector 2
typedef union {
	struct {
		double x, y;
	};
	struct {
		double w, h;
	};
	double raw[2];
} vec2;

// vector 3
typedef union {
	struct {
		double x, y, z;
	};
	struct {
		double r, g, b;
	};
	double raw[3];
} vec3;

#define VEC2I(X, Y)  ((vec2i){{X, Y}})
#define VEC2(X, Y)  ((vec2){{X, Y}})
#define VEC3(X, Y, Z)  ((vec3){{X, Y, Z}})

double random();

double clamp(double, double, double);

vec3 vec3_add(vec3, vec3);
vec3 vec3_sub(vec3, vec3);
vec3 vec3_mul(vec3, vec3);
vec3 vec3_mulf(vec3, double);
vec3 vec3_divf(vec3, double);

double vec3_dist(vec3, vec3);
double vec3_length(vec3);
vec3 vec3_normalize(vec3);
vec3 vec3_cross(vec3, vec3);
double vec3_dot(vec3, vec3);

#endif // math_h__
