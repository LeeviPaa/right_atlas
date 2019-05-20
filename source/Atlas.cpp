#include "Atlas.h"
#include <iostream>
#include <fstream>
#include <math.h>

void Atlas::SaveAtlas(char* filename, char* title, std::vector<PngImage *> &images)
{
	int code = 0;
	FILE *fp = NULL;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	png_bytep row = NULL;
	xPos = 0, yPos = 0;
	int x, y;

	totalImageCount = images.size();
	verticalImageCount = (int)std::sqrt(totalImageCount);
	
	// Open file for writing (binary mode)
	fp = fopen(filename, "wb");
	if (fp == NULL) {
	   fprintf(stderr, "Could not open file %s for writing\n", filename);
	   code = 1;
	  goto finalise;
	}
	
	// Initialize write structure
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
	   fprintf(stderr, "Could not allocate write struct\n");
	   code = 1;
	   goto finalise;
	}
	
	// Initialize info structure
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
	   fprintf(stderr, "Could not allocate info struct\n");
	   code = 1;
	   goto finalise;
	}
	
	// Setup Exception handling
	if (setjmp(png_jmpbuf(png_ptr))) {
	   fprintf(stderr, "Error during png creation\n");
	   code = 1;
	   goto finalise;
	}
	
	width = 0;
	height = 0;
	
	if(verticalImageCount == 0)
	   verticalImageCount = 1;
	
	//TODO: Define positions here!
	for(int i=0; i<images.size(); ++i)
	{
		PngImage* img = images[i];
	
		if(i % verticalImageCount == 0)
		{
			height += img->height;
		}
		else
			width += img->width;
			
	}
	
	png_init_io(png_ptr, fp);
	
	// Write header (8 bit colour depth)
	png_set_IHDR(png_ptr, info_ptr, width, height,
	      8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
	      PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);
	
	// Set title
	if (title != NULL) {
	   png_text title_text;
	   title_text.compression = PNG_TEXT_COMPRESSION_NONE;
	   title_text.key = "Title";
	   title_text.text = title;
	   png_set_text(png_ptr, info_ptr, &title_text, 1);
	}
	
	png_write_info(png_ptr, info_ptr);
	
	// Allocate array for one row (3 bytes per pixel - RGB)
	row_pointers = new png_bytep[height];
	for (y=0; y<height; y++)
		row_pointers[y] = new png_byte[3 * width];
	
	//write images to the atlas
	
	for(int i=0; i<images.size(); ++i)
	{
		PngImage* img = images[i];
	
		if(img->height > largestHeight)
			largestHeight = img->height;
	
		if(xPos + img->width > width)
		{
			xPos = 0;
			yPos += largestHeight;
			largestHeight = 0;
		}
	
		img->posX = xPos;
		img->posY = yPos;
	
		for (y = 0 ; y < img->height ; y++) 
		{
			png_byte* rowCopy = img->row_pointers[y];
			png_byte* row = row_pointers[yPos+y];
	
			for (x = 0 ; x < img->width ; x++) {
				png_byte* ptr = &(row[(xPos+x)*3]);
	
				png_byte* ptrCopy = NULL;
	
				if(img->color_type == 6)
					ptrCopy = &(rowCopy[x*4]);
				else if(img->color_type == 2)
					ptrCopy = &(rowCopy[x*3]);
				else
				{
					std::cout << "invalid image color type!" << (int)img->color_type << std::endl; 
					return;
				}
			
				ptr[0] = ptrCopy[0];
				ptr[1] = ptrCopy[1];
				ptr[2] = ptrCopy[2];
			}
		}
		xPos += img->width;
	}
	
	png_write_image(png_ptr, row_pointers);
	
	// End write
	png_write_end(png_ptr, NULL);
	
	finalise:
	if (fp != NULL) fclose(fp);
	if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	if (row != NULL) free(row);
}

void Atlas::SaveMetadata(std::vector<PngImage*> &images)
{
	std::ofstream file;
	file.open ("metadata.txt");
	for(int i=0; i<images.size(); ++i)
	{
		PngImage* img = images[i];
		file << "name="<< img->name;
		file << " width="<< img->width;
		file << " height="<< img->height;
		file << " x="<< img->posX;
		file << " y="<< img->posY;
		file <<"\n";
	}
	file.close();
}

