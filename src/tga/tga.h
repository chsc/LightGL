/*
 *
 */

#ifndef TGA_H_INCLUDED
#define TGA_H_INCLUDED

typedef struct TGA_s {
	unsigned int* pixels;
	unsigned int  width;
	unsigned int  height;
}TGA;

TGA* tgaLoad(const char* file);
void tgaUnload(TGA* data);

#endif
