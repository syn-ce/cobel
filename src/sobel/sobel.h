#ifndef SOBEL
#define SOBEL

#include <png.h>

void apply_sobel_edge_detection(int black_white, double S_X[3][3], double S_Y[3][3], int width, int height, png_bytep *row_pointers);

#endif // !SOBEL
