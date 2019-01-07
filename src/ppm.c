
#include <stdio.h>

#include "ppm.h"


int ppm_write(char *filename, int w, int h, void *data, size_t size) {

	FILE *file = fopen(filename, "wb");

	if(file == NULL) {
		return -1;
	}

	fprintf(file, "P6\n%d %d\n%d\n", w, h, 255);

	fwrite(data, size, 1, file);
	fclose(file);

	return 0;
}
