#ifndef PNG_UTILS
#define PNG_UTILS

#include <png.h>

void read_png_file(char *filename, int *width, int *height, png_byte *color_type, png_byte *bit_depth, png_bytep **row_pointers);

void write_png_file(char *filename, int width, int height, png_bytep *row_pointers);

void free_row_pointers(png_bytep *row_pointers, int height);

#endif // !PNG_UTILS
