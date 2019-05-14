#include <iostream>
#include "PngImage.h"

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
	if (argc != 3)
	{
		abortt_("Usage: program_name <file_in> <file_out>");
	}

	PngImage image;
	image.OpenImage(argv[1]);
	image.process_file();
	image.WritePngFile(argv[2]);
	return 0;
}