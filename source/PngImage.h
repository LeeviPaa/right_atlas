#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <iostream>

#define PNG_DEBUG 3
#include <png.h>
#include <string>

class PngImage
{
public:
	std::string name;
	int width, height, area;
	int posX, posY;
	
	png_byte color_type;
	png_byte bit_depth;
	
	png_structp png_ptr;
	png_infop info_ptr;
	int number_of_passes;
	png_bytep * row_pointers;

	PngImage(){ 
		row_pointers = nullptr;
	}
	~PngImage();
	int OpenImage(const char* file_path, const char* file_name);
	void SavePngFile(char* location);
	void process_file(void);

	static bool CompareImageSize(PngImage* a, PngImage* b) { return (a->area > b->area); }
};

