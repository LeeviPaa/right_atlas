#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define PNG_DEBUG 3
#include <png.h>

class PngImage
{
public:
	int width, height;

	PngImage(void){}
	void OpenImage(char* location);
	void WritePngFile(char* location);
	void process_file(void);

private:
	int x, y;
	png_byte color_type;
	png_byte bit_depth;
	
	png_structp png_ptr;
	png_infop info_ptr;
	int number_of_passes;
	png_bytep * row_pointers;
};

