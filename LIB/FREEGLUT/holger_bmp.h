#ifndef _HOLGER_BMP
#define _HOLGER_BMP

/*
 * Holger BMP
 *
 * Provides tools for loading and saving 24 bits bmp ("bitmap") files
 *
 * Written strictly ad hoc
 *
 * Author: Holger Ludvigsen
*/

#include <stdio.h>
#include <math.h>
#include <stdlib.h>

typedef struct {
	unsigned char r; // Red component
	unsigned char g; // Blue component
	unsigned char b; // Green component
} pixel;

typedef struct {
	int w; // Width
	int h; // Height
	pixel *pixels; // Array of pixels (coordinates (x, y) is pixels[y*w + x])
} pixel_matrix;

void print_pixel(pixel p) {
	printf("%d,%d,%d ", p.r/255, p.g/255, p.b/255);
}

void print_matrix(pixel_matrix matrix) {
	int y, x;
	for (y = 0; y < matrix.h; y++) {
		for (x = 0; x < matrix.h; x++) {
			print_pixel(matrix.pixels[y*matrix.w + x]);
		}
		printf("\n");
	}
}

// Internally used
void write_little_endian(FILE* fpw, int value, int n_bytes) {
    int i;
	for (i = 0; i < n_bytes; i++) {	
		fputc((value % 256), fpw);
		if (value >= 256) {
			value -= value % 256;
			value /= 256;
		} else {
			value = 0;
		}
	}
}

// Internally used
int read_little_endian(FILE* fpr, int n_bytes) {
	int value = 0;
	int i;
	unsigned char c;
	for (i = 0; i < n_bytes; i++) {
		c = fgetc(fpr);
		value += (int) pow(256.0f, i)*c;
	}

	return value;
}

pixel_matrix load_bmp(char* filename) {
	FILE* fpr = fopen(filename, "rb");

	int w, h;

	// Read file header
	int i = 0;
	while (i <= 53) { // Header is 54 bits long
		if (i == 18) { // Read width
			w = read_little_endian(fpr, 4);
			i += 4;
		} else if (i == 22) { // Read height
			h = read_little_endian(fpr, 4);
			i += 4;
		} else {
			fgetc(fpr);
			i++;
		}
	}

	pixel_matrix matrix;
	matrix.w = w;
	matrix.h = h;
	matrix.pixels = (pixel*)(malloc(sizeof(pixel)*w*h));

	// Read bitmap
	int x, y, p;
	pixel pix;
	for (y = h-1; y >= 0; y--) { // Bitmap is stored up-side-down
		for (x = 0; x < w; x++) { // Read one row of pixels
			pix.b = getc(fpr);
			pix.g = getc(fpr);
			pix.r = getc(fpr);
			
			matrix.pixels[y*w + x] = pix;
		}

		if ((w*3)%4 != 0) { // Read padding if number of bytes in row is not multiple of four as specified in bmp documentation
			for (p = 0; p < 4-(w*3)%4; p++) { 
				fgetc(fpr);
			}
		}
	}
	
	fclose(fpr);

	return matrix;
}

unsigned int * load_bmp_int(char* filename, int * width, int * height) {
	pixel_matrix matrix = load_bmp(filename);
	unsigned int * int_matrix = (unsigned int *) malloc(matrix.h * matrix.w * sizeof(unsigned int));
	*width = matrix.w;
	*height = matrix.h;

	int x, y;
	for (y = 0; y < matrix.h; y++) {
		for (x = 0; x < matrix.w; x++) {
			//int_matrix[y*matrix.w + x] = 0x00000000;
			pixel pix = matrix.pixels[(matrix.h-1-y)*matrix.w + x];
			int_matrix[y*matrix.w + x] = pix.b;
			int_matrix[y*matrix.w + x] <<= 8;
			int_matrix[y*matrix.w + x] |= pix.g;
			int_matrix[y*matrix.w + x] <<= 8;
			int_matrix[y*matrix.w + x] |= pix.r;
		}
	}

	return int_matrix;
}

unsigned char * load_bmp_char(char* filename, int * width, int * height) {
	pixel_matrix matrix = load_bmp(filename);
	unsigned char * int_matrix = (unsigned char *) malloc(3*matrix.h * matrix.w * sizeof(unsigned char));
	*width = matrix.w;
	*height = matrix.h;

	int x, y;
	for (y = 0; y < matrix.h; y++) {
		for (x = 0; x < matrix.w; x++) {
			//int_matrix[y*matrix.w + x] = 0x00000000;
			pixel pix = matrix.pixels[(matrix.h-1-y)*matrix.w + x];
            int pos = 3*(y*matrix.w + x);
			int_matrix[pos+2] = pix.b;
			int_matrix[pos+1] = pix.g;
			int_matrix[pos] = pix.r;
		}
	}

	return int_matrix;
}
float * load_bmp_grey_float(char * filename, int * width, int * height) {
	pixel_matrix matrix = load_bmp(filename);
	float * float_matrix = (float *) malloc(matrix.h * matrix.w * sizeof(float));
	*width = matrix.w;
	*height = matrix.h;

	int x, y;
	for (y = 0; y < matrix.h; y++) {
		for (x = 0; x < matrix.w; x++) {
			pixel pix = matrix.pixels[(matrix.h-1-y)*matrix.w + x];
			float_matrix[y*matrix.w + x] = (pix.b + pix.g + pix.r)/3.0/256.0;
			//if (x < 50 && y < 50) printf("float:%f\n", (pix.b + pix.g + pix.r)/3.0/256.0);
		}
	}

	return float_matrix;
}

void greyfloat_to_rgbuint(float * grey, unsigned int * rgb, int w, int h) {
	int x, y;
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			//if (x < 50 && y < 50) printf("fooo\n");
			if (grey[y*w + x] >= 1) grey[y*w + x] = 0.999;
			if (grey[y*w + x] < 0) grey[y*w + x] = 0;
			int greytone = grey[y*w + x]*256;
			rgb[y*w + x] = greytone | (greytone<<8) | (greytone<<16);
		}
	}
}

/*rgbint_to_greyfloat(float * grey, int * rgb, int w, int h) {
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			unsigned char red = rgb[y*w + x];
			unsigned char blue = rgb[y*w + x] >> 8;
			unsigned char green = rgb[y*w + x] >> 16;
			grey[y*w + x] = (red + blue + green)/3.0f;
		}
	}
}*/

void save_bmp(pixel_matrix matrix, char* filename) {
	FILE* fpw = fopen(filename, "wb");

	int w = matrix.w;
	int h = matrix.h;
	pixel *pixels = matrix.pixels;

	// Write file header
	int i = 0;
	while (i <= 53) { // Header is 54 bits long
		if (i == 0) { // Write "BM" ("BM" for "BitMap", standard start of bmp file)
			fputc(66, fpw);
			fputc(77, fpw);
			i += 2;
		} else if (i == 2) { // Write size of file
			int size = 54 + h*3*w; // size = header + height*3*width
			if ((w*3)%4 != 0) size += (4-(w*3)%4)*h;  // If padding (according to bmp specification) is needed, add it to size
			write_little_endian(fpw, size, 4);
			i += 4;
		} else if (i == 10) { // Write "54" (offset from beginning of file to bitmap data)
			fputc(54, fpw);
			i++;
		} else if (i == 14) { // Write "40" (length of rest of header)
			fputc(40, fpw);
			i++;
		} else if (i == 18) { // Write width
			write_little_endian(fpw, w, 4);
			i += 4;
		} else if (i == 22) { // Write height
			write_little_endian(fpw, h, 4);
			i += 4;
		} else if (i == 26) { // Write "1" (number of planes of the target device)
			fputc(1, fpw);
			i++;
		} else if (i == 28) { // Write "24" (number of bits per pixel)
			fputc(24, fpw);
			i++;
		} else {
			fputc(0, fpw);
			i++;
		}
	}

	// Write bitmap
	int x, y, p;
	pixel pix;
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) { // Write one row of pixels
			pix = pixels[y*w + x];
			fputc(pix.b, fpw);
			fputc(pix.g, fpw);
			fputc(pix.r, fpw);
		}
		if ((w*3)%4 != 0) { // Write padding if number of bytes in the row is not multiple of four
			for (p = 0; p < 4-(w*3)%4; p++) { 
				fputc(0, fpw);
			}
		}
	}
	
	fclose(fpw);
}

/*void save_bmp(float3 * pixels, int w, int h, char* filename) {
	pixel_matrix matrix;
	matrix.w = w;
	matrix.h = h;
	matrix.pixels = (pixel*)(malloc(sizeof(pixel)*w*h));

	int x, y;
	pixel pix;
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			pix.b = (unsigned char) (pixels[y*w + x].x*256);
			pix.g = (unsigned char) (pixels[y*w + x].y*256);
			pix.r = (unsigned char) (pixels[y*w + x].z*256);
			
			matrix.pixels[y*w + x] = pix;
		}
	}
	
	save_bmp(matrix, filename);
}
*/

/*void save_bmp(uchar4 * pixels, int w, int h, char* filename) {
	pixel_matrix matrix;
	matrix.w = w;
	matrix.h = h;
	matrix.pixels = (pixel*)(malloc(sizeof(pixel)*w*h));

	int x, y;
	pixel pix;
	for (y = 0; y < h; y++) {
		for (x = 0; x < w; x++) {
			pix.b = (unsigned char) (pixels[y*w + x].x);
			pix.g = (unsigned char) (pixels[y*w + x].y);
			pix.r = (unsigned char) (pixels[y*w + x].z);
			
			matrix.pixels[y*w + x] = pix;
		}
	}
	
	save_bmp(matrix, filename);
}
*/

// Convert pixel_matrix to grey tones (in place)
void greyify(pixel_matrix matrix) {
	int w = matrix.w;
	int h = matrix.h;
	pixel* pixels = (pixel*)matrix.pixels;

    int x,y;
	for (x = 0; x < w; x++) {
		for (y = 0; y < h; y++) {
			pixel p = pixels[x + y*w];
			int intensity = (p.r + p.g + p.b) / 3;
			pixels[x + y*w].r = intensity;
			pixels[x + y*w].g = intensity;
			pixels[x + y*w].b = intensity;
		}
	}
}

#endif
