#include <math.h>
#include <png.h>
#include <stdlib.h>

void apply_sobel_edge_detection(int color, double S_X[3][3], double S_Y[3][3], int width, int height, png_bytep *row_pointers) {
    png_bytep prev_sobel_pixels = malloc(sizeof(png_byte) * width * 4);
    png_bytep curr_prev = malloc(sizeof(png_byte) * 3);
    curr_prev[0] = row_pointers[0][3];
    curr_prev[1] = row_pointers[0][4];
    curr_prev[2] = row_pointers[0][5];


    for (int i = 0; i < width * 4; i++) {
        prev_sobel_pixels[i] = row_pointers[0][i];
    }

    for(int y = 1; y < height - 1; y++) {
        png_bytep prev_row = row_pointers[y-1];
        png_bytep curr_row = row_pointers[y];
        png_bytep next_row = row_pointers[y+1];
        
        for(int x = 1; x < width - 1; x++) {
            png_bytep px00 = &(prev_row[(x - 1) * 4]);
            png_bytep px01 = &(curr_row[(x - 1) * 4]);
            png_bytep px02 = &(next_row[(x - 1) * 4]);
            png_bytep px10 = &(prev_row[x * 4]);
            png_bytep px11 = &(curr_row[x * 4]);
            png_bytep px12 = &(next_row[x * 4]);
            png_bytep px20 = &(prev_row[(x + 1) * 4]);
            png_bytep px22 = &(next_row[(x + 1) * 4]);
            png_bytep px21 = &(curr_row[(x + 1) * 4]);

            double G_xr = (S_X[0][0] * px00[0]) + (S_X[0][1] * px10[0]) + (S_X[0][2] * px20[0]) +
	  	  		              (S_X[1][0] * px01[0]) + (S_X[1][1] * px11[0]) + (S_X[1][2] * px21[0]) +
	  	  		              (S_X[2][0] * px02[0]) + (S_X[2][1] * px12[0]) + (S_X[2][2] * px22[0]);

            double G_yr = (S_Y[0][0] * px00[0]) + (S_Y[0][1] * px10[0]) + (S_Y[0][2] * px20[0]) +
	  	  		              (S_Y[1][0] * px01[0]) + (S_Y[1][1] * px11[0]) + (S_Y[1][2] * px21[0]) +
	  	  		              (S_Y[2][0] * px02[0]) + (S_Y[2][1] * px12[0]) + (S_Y[2][2] * px22[0]);

            double G_xg = (S_X[0][0] * px00[1]) + (S_X[0][1] * px10[1]) + (S_X[0][2] * px20[1]) +
	  	  		              (S_X[1][0] * px01[1]) + (S_X[1][1] * px11[1]) + (S_X[1][2] * px21[1]) +
	  	  		              (S_X[2][0] * px02[1]) + (S_X[2][1] * px12[1]) + (S_X[2][2] * px22[1]);

            double G_yg = (S_Y[0][0] * px00[1]) + (S_Y[0][1] * px10[1]) + (S_Y[0][2] * px20[1]) +
	  	  		              (S_Y[1][0] * px01[1]) + (S_Y[1][1] * px11[1]) + (S_Y[1][2] * px21[1]) +
	  	  		              (S_Y[2][0] * px02[1]) + (S_Y[2][1] * px12[1]) + (S_Y[2][2] * px22[1]);

            double G_xb = (S_X[0][0] * px00[2]) + (S_X[0][1] * px10[2]) + (S_X[0][2] * px20[2]) +
	  	  		              (S_X[1][0] * px01[2]) + (S_X[1][1] * px11[2]) + (S_X[1][2] * px21[2]) +
	  	  		              (S_X[2][0] * px02[2]) + (S_X[2][1] * px12[2]) + (S_X[2][2] * px22[2]);

            double G_yb = (S_Y[0][0] * px00[2]) + (S_Y[0][1] * px10[2]) + (S_Y[0][2] * px20[2]) +
	  	  		              (S_Y[1][0] * px01[2]) + (S_Y[1][1] * px11[2]) + (S_Y[1][2] * px21[2]) +
	  	  		              (S_Y[2][0] * px02[2]) + (S_Y[2][1] * px12[2]) + (S_Y[2][2] * px22[2]);


            int Gr = (int) sqrt(G_xr * G_xr + G_yr * G_yr);
            int Gg = (int) sqrt(G_xg * G_xg + G_yg * G_yg);
            int Gb = (int) sqrt(G_xb * G_xb + G_yb * G_yb);

            // Do something awesome for each pixel here...
            //printf("%4d, %4d =[0]GBA(%3d, %3d, %3d, %3d)\n", x, y, px[0], px[1], px[2], px[3]);
            px00[0] = prev_sobel_pixels[(x-1) * 4 + 0];
            px00[1] = prev_sobel_pixels[(x-1) * 4 + 1];
            px00[2] = prev_sobel_pixels[(x-1) * 4 + 2];

            prev_sobel_pixels[(x - 1) * 4] = curr_prev[0];
            prev_sobel_pixels[(x - 1) * 4 + 1] = curr_prev[1];
            prev_sobel_pixels[(x - 1) * 4 + 2] = curr_prev[2];
            if (!color) {
                curr_prev[0] = curr_prev[1] = curr_prev[2] = (Gr + Gg + Gb) / 3;
            } 
            else {
                curr_prev[0] = Gr;
                curr_prev[1] = Gg;
                curr_prev[2] = Gb;
            }
        }
        // Second-to-last column pixel (last pixel being changed in column)
        row_pointers[y-1][(width - 2) * 4 + 0] = curr_prev[0];
        row_pointers[y-1][(width - 2) * 4 + 1] = curr_prev[1]; 
        row_pointers[y-1][(width - 2) * 4 + 2] = curr_prev[2];

        curr_prev[0] = row_pointers[y][0];
        curr_prev[1] = row_pointers[y][1];
        curr_prev[2] = row_pointers[y][2];
    }
    // Second-to-last row (last row being colored) 
    for (int x = 1; x < width - 1; x++) {
        row_pointers[height - 2][x * 4] = curr_prev[0];
        row_pointers[height - 2][x * 4 + 1] = curr_prev[1];
        row_pointers[height - 2][x * 4 + 2] = curr_prev[2];
    }

    free(prev_sobel_pixels);
    free(curr_prev);
}
