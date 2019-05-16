#include <iostream>
#include "PngImage.h"

#ifdef _WIN32
#include "dirent.h"
#endif

#ifdef linux
#include <dirent.h>
#endif

void abortt_(const char * s, ...)
{
        va_list args;
        va_start(args, s);
        vfprintf(stderr, s, args);
        fprintf(stderr, "\n");
        va_end(args);
        abort();
}

int main (int argc, char* argv[])
{
	DIR *dir;
	struct dirent *ent;
	if ((dir = opendir (argv[1])) != NULL) {
	  /* print all the files and directories within directory */
	  while ((ent = readdir (dir)) != NULL) {
	    //printf ("%s\n", ent->d_name);
		char *output = NULL;
		output = strstr (ent->d_name,".png");
		if(output) {
		    printf ("%s\n", ent->d_name);
		}
	  }
	  closedir (dir);
	} else {
	  /* could not open directory */
	  perror ("");
	  return -1;
	}
	/*if (argc != 3)
	{
		abortt_("Usage: program_name <file_in> <file_out>");
	}

	PngImage image;
	image.OpenImage(argv[1]);
	image.process_file();
	image.WritePngFile(argv[2]);*/
	system("pause");
	return 0;
}