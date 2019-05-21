#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "PngImage.h"
#include "Atlas.h"

#ifdef _WIN32
#include "dirent.h"
#endif

#ifdef linux
#include <dirent.h>
#endif

int main (int argc, char* argv[])
{
	if (argc != 2)
	{
		std::cout << "Parameters: <image_location>" << std::endl;
		return -1;
	}

	DIR *dir;
	struct dirent *ent;
	std::vector<PngImage*> images;
	Atlas atlas;
	std::string path = argv[1];

	if ((dir = opendir (path.c_str())) != NULL) {
	  while ((ent = readdir (dir)) != NULL) {

		char *output = NULL;
		output = strstr (ent->d_name,".png");

		std::string name(ent->d_name);

		std::size_t found = name.find(".png");
		if(found == std::string::npos)
			found = name.find(".PNG");

		if(found != std::string::npos) {
		    std::cout << "found png: " <<ent->d_name << std::endl;
			PngImage* image = new PngImage();

			std::string imgPath = "";
			imgPath.append(path);

			if(imgPath.back() != '/')
				imgPath.append("/");

			imgPath.append(ent->d_name);

			if(image->OpenImage(imgPath.c_str(), ent->d_name) == 0)
				images.push_back(image);
			else
				delete(image);
		}
	  }
	  closedir (dir);
	  std::sort(images.begin(), images.end(), PngImage::CompareImageSize);
	} 
	else 
	{
	  std::cout << "could not open directory" << std::endl;
	  return -1;
	}


	if(images.size() > 0)
	{
		PngImage* imgPtr = images[0];
		atlas.CreateAtlas(images);
		atlas.SavePngFile("atlas.png");
		atlas.SaveMetadata("metadata.txt",images);
	}
	else
	{
		std::cout << "no png files found" << std::endl;
		return -1;
	}

	for(int i=0; i<images.size(); ++i)
		delete(images[i]);

	return 0;
}