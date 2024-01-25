#ifndef SOBEL
#define SOBEL

#include <png.h>

void apply_sobel_edge_detection(int black_white, int width, int height, png_bytep *row_pointers);

#endif // !SOBEL
