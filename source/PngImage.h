#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <iostream>

#define PNG_DEBUG 3
#include <png.h>

class PngImage
{
public:
	int width, height, area;

	PngImage(void){}
	int OpenImage(const char* location);
	void SavePngFile(char* location);
	void process_file(void);

	int x, y;
	int posX, posY;
	png_byte color_type;
	png_byte bit_depth;
	
	png_structp png_ptr;
	png_infop info_ptr;
	int number_of_passes;
	png_bytep * row_pointers;

	static bool CompareImageSize(PngImage* a, PngImage* b) { return (a->area > b->area); }

};

