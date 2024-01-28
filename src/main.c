#include <png.h>
#include <stdio.h>
#include <stdlib.h>
#include "png/pngutils.h"
#include "sobel/sobel.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Specify the filename of the image to read.\n");
        exit(1);
    }

    // -- Command line arguments
    char *filename = argv[1];

    int color = 0;
    if (argc > 2) color = atoi(argv[2]);

    char *out = "output.png";
    if (argc > 3) out = argv[3];

    int nr_iterations = 1;
    if (argc > 4) nr_iterations = atoi(argv[4]);


    double S_Y[3][3] = {{-1,0,1}, {-2,0,2}, {-1,0,1}}; // Hardcoded for now
    double S_X[3][3] = {{-1,-2,-1}, {0,0,0}, {1,2,1}};

    int arg_nr = 5;
    int i = 0;
    while (argc > arg_nr && i < 9) {
        S_X[i / 3][i % 3] = atoi(argv[arg_nr - 1]);
        arg_nr++;
        i++;
    }

    i = 0;
    while (argc > arg_nr && i < 9) {
        S_Y[i / 3][i % 3] = atoi(argv[arg_nr - 1]);
        arg_nr++;
        i++;
    }
    // --

    int width, height;
    png_byte color_type;
    png_byte bit_depth;
    png_bytep *row_pointers = NULL;

    read_png_file(filename, &width, &height, &color_type, &bit_depth, &row_pointers);
    for (int i = 0; i < nr_iterations; i++)
        apply_sobel_edge_detection(color, S_X, S_Y, width, height, row_pointers);
    write_png_file(out, width, height, row_pointers);

    free_row_pointers(row_pointers, height);

    return 0;
}

