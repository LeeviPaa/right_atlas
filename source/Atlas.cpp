#include "Atlas.h"
#include <iostream>
#include <fstream>
#include <math.h>

void Atlas::CreateAtlas(char* filename, char* title, std::vector<PngImage *> &images)
{
	xPos = 0, yPos = 0;
	int x, y;

	totalImageCount = images.size();
	verticalImageCount = (int)std::sqrt(totalImageCount);
	
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

