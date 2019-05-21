#include "Atlas.h"
#include <iostream>
#include <fstream>
#include <math.h>

void Atlas::CreateAtlas(std::vector<PngImage *> &images)
{
	xPos = 0, yPos = 0;
	width = 0, height = 0;
	float totalArea = 0, approximateWidth = 0;

	for(int i=0; i<images.size(); ++i)
	{
		PngImage* img = images[i];
		totalArea += img->area;
	}

	approximateWidth = (int)std::sqrt(totalArea);

	//we define the dimensons of the atlas and positions of the images
	int maxHeight = 0, maxWidth = 0;
	for(int i=0; i<images.size(); ++i)
	{
		PngImage* img = images[i];
		img->posX = 0;
		img->posY = 0;

		if(img->height > maxHeight)
			maxHeight = img->height;

		img->posX = xPos;

		if(xPos > maxWidth)
				maxWidth = xPos;

		xPos += img->width;

		if(xPos > approximateWidth)
		{
			height += maxHeight;

			yPos += maxHeight;

			xPos = img->width;
			img->posX = 0;
			maxHeight = 0;
		}
		
		img->posY = yPos;

	}
	height += maxHeight;
	width = maxWidth;

	bit_depth = 8;
	color_type = PNG_COLOR_TYPE_RGB;
	
	// Allocate array for row pointers and delete old arrays if they exist
	if(row_pointers != nullptr)
	{
		for (int y=0; y<height; y++)
			delete[](row_pointers[y]);

		delete[](row_pointers);
	}

	row_pointers = new png_bytep[height];
	for (y=0; y<height; y++)
		row_pointers[y] = new png_byte[3 * width];
	
	//write images to the atlas
	for(int i=0; i<images.size(); ++i)
	{
		PngImage* img = images[i];

		int currX = img->posX;
		int currY = img->posY;
	
		for (int y = 0 ; y < img->height ; y++) 
		{
			png_byte* rowCopy = img->row_pointers[y];
			png_byte* row = row_pointers[currY+y];
	
			for (int x = 0 ; x < img->width ; x++) {
				png_byte* ptr = &(row[(currX+x)*3]);
	
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
	}
}

void Atlas::SaveMetadata(const char* filepath, std::vector<PngImage*> &images)
{
	std::ofstream file;
	file.open (filepath);
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

