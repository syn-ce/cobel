/*
 * A simple libpng example program
 * http://zarb.org/~gc/html/libpng.html
 *
 * Modified by Yoshimasa Niwa to make it much simpler
 * and support all defined color_type.
 *
 * To build, use the next instruction on OS X.
 * $ brew install libpng
 * $ clang -lz -lpng16 libpng_test.c
 *
 * Copyright 2002-2010 Guillaume Cottenceau.
 *
 * This software may be freely redistributed under the terms
 * of the X11 license.
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <png.h>
#include <math.h>

int width, height;
png_byte color_type;
png_byte bit_depth;
png_bytep *row_pointers = NULL;

void read_png_file(char *filename) {
  FILE *fp = fopen(filename, "rb");

  png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(!png) printf("no png\n"), abort();

  png_infop info = png_create_info_struct(png);
  if(!info) abort();

  if(setjmp(png_jmpbuf(png))) abort();

  png_init_io(png, fp);

  png_read_info(png, info);

  width      = png_get_image_width(png, info);
  height     = png_get_image_height(png, info);
  color_type = png_get_color_type(png, info);
  bit_depth  = png_get_bit_depth(png, info);

  // Read any color_type into 8bit depth, RGBA format.
  // See http://www.libpng.org/pub/png/libpng-manual.txt

  if(bit_depth == 16)
    png_set_strip_16(png);

  if(color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png);

  // PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
  if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png);

  if(png_get_valid(png, info, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png);

  // These color_type don't have an alpha channel then fill it with 0xff.
  if(color_type == PNG_COLOR_TYPE_RGB ||
     color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

  if(color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png);

  png_read_update_info(png, info);

  if (row_pointers) abort();

  row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
  for(int y = 0; y < height; y++) {
    row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
  }

  png_read_image(png, row_pointers);

  fclose(fp);

  png_destroy_read_struct(&png, &info, NULL);
}

void write_png_file(char *filename) {
  int y;

  FILE *fp = fopen(filename, "wb");
  if(!fp) abort();

  png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png) abort();

  png_infop info = png_create_info_struct(png);
  if (!info) abort();

  if (setjmp(png_jmpbuf(png))) abort();

  png_init_io(png, fp);

  // Output is 8bit depth, RGBA format.
  png_set_IHDR(
    png,
    info,
    width, height,
    8,
    PNG_COLOR_TYPE_RGBA,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
  );
  png_write_info(png, info);

  // To remove the alpha channel for PNG_COLOR_TYPE_RGB format,
  // Use png_set_filler().
  //png_set_filler(png, 0, PNG_FILLER_AFTER);

  if (!row_pointers) abort();

  png_write_image(png, row_pointers);
  png_write_end(png, NULL);

  for(int y = 0; y < height; y++) {
    free(row_pointers[y]);
  }
  free(row_pointers);

  fclose(fp);

  png_destroy_write_struct(&png, &info);
}

// Handle the last two columns and rows which be default are not affected / processed by sobel edge detection.
// For now, only blacks out these last two.
void sobel_edge_last_two() {
    for (int y = height - 2; y < height; y++) {
        png_bytep row = row_pointers[y];
        for (int x = 0; x < width; x++) {
            png_bytep px = &(row[x * 4]);
            px[0] = px[1] = px[2] = 0;
        }
    }
    for (int y = 0; y < height; y++) {
        png_bytep row = row_pointers[y];
        for (int x = width - 2; x < width; x++) {
            png_bytep px = &(row[x * 4]);
            px[0] = px[1] = px[2] = 0;
        }
    }
}

void apply_sobel_edge_detection(int black_white) {
  double S_Y[3][3] = {{-1,0,1}, {-2,0,2}, {-1,0,1}};
  double S_X[3][3] = {{-1,-2,-1}, {0,0,0}, {1,2,1}};

  for(int y = 0; y < height - 2; y++) {
    png_bytep row = row_pointers[y];
    png_bytep row1 = row_pointers[y+1];
    png_bytep row2 = row_pointers[y+2];
    
    for(int x = 0; x < width - 2; x++) {
        png_bytep px00 = &(row[x * 4]);
        png_bytep px01 = &(row1[x * 4]);
        png_bytep px02 = &(row2[x * 4]);
        png_bytep px10 = &(row[(x+1) * 4]);
        png_bytep px20 = &(row[(x+2) * 4]);
        png_bytep px11 = &(row1[(x+1) * 4]);
        png_bytep px22 = &(row2[(x+2) * 4]);
        png_bytep px12 = &(row2[(x+1) * 4]);
        png_bytep px21 = &(row1[(x+2) * 4]);

        double G_xr =   (S_X[0][0] * px00[0]) + (S_X[0][1] * px10[0]) + (S_X[0][2] * px20[0]) +
				        (S_X[1][0] * px01[0]) + (S_X[1][1] * px11[0]) + (S_X[1][2] * px21[0]) +
				        (S_X[2][0] * px02[0]) + (S_X[2][1] * px12[0]) + (S_X[2][2] * px22[0]);

        double G_yr =   (S_Y[0][0] * px00[0]) + (S_Y[0][1] * px10[0]) + (S_Y[0][2] * px20[0]) +
				        (S_Y[1][0] * px01[0]) + (S_Y[1][1] * px11[0]) + (S_Y[1][2] * px21[0]) +
				        (S_Y[2][0] * px02[0]) + (S_Y[2][1] * px12[0]) + (S_Y[2][2] * px22[0]);

        double G_xg =   (S_X[0][0] * px00[1]) + (S_X[0][1] * px10[1]) + (S_X[0][2] * px20[1]) +
				        (S_X[1][0] * px01[1]) + (S_X[1][1] * px11[1]) + (S_X[1][2] * px21[1]) +
				        (S_X[2][0] * px02[1]) + (S_X[2][1] * px12[1]) + (S_X[2][2] * px22[1]);

        double G_yg =   (S_Y[0][0] * px00[1]) + (S_Y[0][1] * px10[1]) + (S_Y[0][2] * px20[1]) +
				        (S_Y[1][0] * px01[1]) + (S_Y[1][1] * px11[1]) + (S_Y[1][2] * px21[1]) +
				        (S_Y[2][0] * px02[1]) + (S_Y[2][1] * px12[1]) + (S_Y[2][2] * px22[1]);

        double G_xb =   (S_X[0][0] * px00[2]) + (S_X[0][1] * px10[2]) + (S_X[0][2] * px20[2]) +
				        (S_X[1][0] * px01[2]) + (S_X[1][1] * px11[2]) + (S_X[1][2] * px21[2]) +
				        (S_X[2][0] * px02[2]) + (S_X[2][1] * px12[2]) + (S_X[2][2] * px22[2]);

        double G_yb =   (S_Y[0][0] * px00[2]) + (S_Y[0][1] * px10[2]) + (S_Y[0][2] * px20[2]) +
				        (S_Y[1][0] * px01[2]) + (S_Y[1][1] * px11[2]) + (S_Y[1][2] * px21[2]) +
				        (S_Y[2][0] * px02[2]) + (S_Y[2][1] * px12[2]) + (S_Y[2][2] * px22[2]);


        int Gr = (int) sqrt(G_xr * G_xr + G_yr * G_yr);
        int Gg = (int) sqrt(G_xg * G_xg + G_yg * G_yg);
        int Gb = (int) sqrt(G_xb * G_xb + G_yb * G_yb);

        // Do something awesome for each pixel here...
        //printf("%4d, %4d =[0]GBA(%3d, %3d, %3d, %3d)\n", x, y, px[0], px[1], px[2], px[3]);
        if (black_white) px00[0] = px00[1] = px00[2] = (Gr + Gg + Gb) / 3;
        else {
            px00[0] = Gr;
            px00[1] = Gg;
            px00[2] = Gb;
        }
    }
  }

  // Process last two columns and rows as well
  sobel_edge_last_two();
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Specify the filename of the image to read.");
        exit(1);
    }

    char *filename = argv[1];

    int black_white = 1;
    if (argc > 2) black_white = atoi(argv[2]);

    char *out = "output.png";
    if (argc > 3) out = argv[3];

    int nr_iterations = 1;
    if (argc > 4) nr_iterations = atoi(argv[4]);

    read_png_file(filename);
    for (int i = 0; i < nr_iterations; i++)
        apply_sobel_edge_detection(black_white);
    write_png_file(out);

    return 0;
}

