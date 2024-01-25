#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include "pngutils.h"
#include "sobel.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Specify the filename of the image to read.\n");
        exit(1);
    }

    // -- Command line arguments
    char *filename = argv[1];

    int black_white = 1;
    if (argc > 2) black_white = atoi(argv[2]);

    char *out = "output.png";
    if (argc > 3) out = argv[3];

    int nr_iterations = 1;
    if (argc > 4) nr_iterations = atoi(argv[4]);
    // --

    int width, height;
    png_byte color_type;
    png_byte bit_depth;
    png_bytep *row_pointers = NULL;

    read_png_file(filename, &width, &height, &color_type, &bit_depth, &row_pointers);
    for (int i = 0; i < nr_iterations; i++)
        apply_sobel_edge_detection(black_white, width, height, row_pointers);
    write_png_file(out, width, height, row_pointers);

    free_row_pointers(row_pointers, height);

    return 0;
}

