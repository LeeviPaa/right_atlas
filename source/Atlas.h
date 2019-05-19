#pragma once
#include "PngImage.h"
#include <vector>

class Atlas : PngImage
{
public:
	void SaveAtlas(char* filename, char* title, std::vector<PngImage *> images);
};

