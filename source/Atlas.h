#pragma once
#include "PngImage.h"
#include <vector>

class Atlas : public PngImage
{
public:
	void CreateAtlas(char* filename, char* title, std::vector<PngImage *> &images);
	void SaveMetadata(std::vector<PngImage*> &images);
private:
	int xPos, yPos;
   	int x, y;
	int largestHeight;
	int verticalImageCount;
	float totalImageCount;

};

