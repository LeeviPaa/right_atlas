#pragma once
#include "PngImage.h"
#include <vector>

class Atlas : public PngImage
{
public:
	void CreateAtlas(std::vector<PngImage *> &images);
	void SaveMetadata(const char* filepath, std::vector<PngImage*> &images);
private:
	int xPos, yPos;
   	int x, y;
	int largestHeight;
	float totalImageCount;

};

