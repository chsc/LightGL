/*
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "tga.h"

#define TGA_OUT "tga: "

static int tgaReadUChar(FILE* f, unsigned char* c) {
	return fread(c, 1, 1, f) == 1;
}

static int tgaReadUShort(FILE* f, unsigned short* us) {
	unsigned char b[2];
	size_t read = fread(b, 2, 1, f);
	*us = b[1] << 8 | b[0];
	return read;
}

static int tgaReadColor(FILE* f, unsigned int* col) {
	unsigned char b[3];
	size_t read = fread(b, 3, 1, f);
	*col = (b[2] << 16) | (b[1] << 8) | b[0];
	return read;
}

TGA* tgaLoad(const char* filename) {
	TGA* tga;
	FILE* file;
	unsigned char lenid;
	unsigned char paltype;
	unsigned char imgtype;
	unsigned short palstart, pallen;
	unsigned char palitmsize;
	unsigned short startx, starty, width, height;
	unsigned char bpp, attr;
	unsigned int col;
	int i, j;

	file = fopen(filename, "rb");
	if (file == NULL) {
		fprintf(stderr, TGA_OUT "Unable to load TGA file: %s\n", filename);
		return NULL;
	}

	tga = malloc(sizeof(TGA));
	if (tga == NULL) {
		fprintf(stderr, TGA_OUT "Unable to allocate TGA struct: %s\n", filename);
		fclose(file);
		return NULL;
	}

	memset(tga, 0, sizeof(TGA));

	printf(TGA_OUT "Loading %s ...\n", filename);

	tgaReadUChar(file, &lenid);
	tgaReadUChar(file, &paltype);
	tgaReadUChar(file, &imgtype);
	tgaReadUShort(file, &palstart);
	tgaReadUShort(file, &pallen);
	tgaReadUChar(file, &palitmsize);
	tgaReadUShort(file, &startx);
	tgaReadUShort(file, &starty);
	tgaReadUShort(file, &width);
	tgaReadUShort(file, &height);
	tgaReadUChar(file, &bpp);
	tgaReadUChar(file, &attr);

#ifndef NDEBUG
	printf(TGA_OUT " Image header:\n");
	printf(TGA_OUT " lenid:%d paltype:%d imgtype:%d\n", (int) lenid, (int) paltype, (int) imgtype);
	printf(TGA_OUT " palstart:%d pallen:%d palitmsize:%d\n", (int) palstart, (int) pallen, (int) palitmsize);
	printf(TGA_OUT " sx:%d sy:%d w:%d h:%d\n", (int) startx, (int) startx, (int) width, (int) height);
	printf(TGA_OUT " bpp:%d attr:%d\n", (int) bpp, (int) attr);
#endif

	if (lenid != 0 || paltype != 0 || imgtype != 2 || palstart != 0 || pallen != 0 || palitmsize != 0 || bpp != 24
			|| attr != 32) {
		fprintf(stderr,
				TGA_OUT  "Unable to load TGA file: %s."
				" Must be an uncompressed RGB (24bit) image in top left format.\n",
				filename);
		free(tga);
		fclose(file);
		return NULL;
	}

	tga->pixels = malloc(sizeof(unsigned int) * width * height);
	if(tga->pixels == NULL) {
		fprintf(stderr, TGA_OUT "Unable to allocate image data for file: %s\n", filename);
		free(tga);
		fclose(file);
		return NULL;
	}

	for (i = 0; i < height; i++) {
		for (j = 0; j < width; j++) {
			tgaReadColor(file, &col);
			tga->pixels[i * width + j] = col;
		}
	}

	fclose(file);
	return tga;
}

void tgaUnload(TGA* data) {
	free(data->pixels);
	free(data);
}
