#pragma once
#include "PngImage.h"
#include <vector>

class Atlas : PngImage
{
public:
	void SaveAtlas(char* filename, char* title, std::vector<PngImage *> &images);
	void SaveMetadata(std::vector<PngImage*> &images);
private:
	int xPos, yPos;
   	int x, y;
	int largestHeight;
	int verticalImageCount;
	float totalImageCount;

};

