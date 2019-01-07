#ifndef halton_h__
#define halton_h__

typedef struct {
	int	base;
	double	inv_base;
	int	i;
} halton_state;

halton_state halton_new(int seed);
double halton_get(halton_state*);

#endif // halton_h__
