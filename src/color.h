#ifndef color_h__
#define color_h__


#include <math.h>
#include "math.h"

typedef struct {
	uint8_t r, g, b;
} rgb8;


rgb8 tonemap(vec3 color, uint32_t samples, double exposure) {

	color.r = (color.r / samples) * exposure;
	color.g = (color.g / samples) * exposure;
	color.b = (color.b / samples) * exposure;

	return (rgb8){
		.r = (uint8_t)(clamp(pow(color.r, 1/2.2), 0, 1) * 255),
		.g = (uint8_t)(clamp(pow(color.g, 1/2.2), 0, 1) * 255),
		.b = (uint8_t)(clamp(pow(color.b, 1/2.2), 0, 1) * 255),
	};
}

#endif // color_h__
