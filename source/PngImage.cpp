/*OpenImage-, WritePngFile-, and process_file-
 *functions based on this example file by Guillaume Cottenceau: http://zarb.org/~gc/html/libpng.html  */

#include "PngImage.h"
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

#define PNG_DEBUG 3
#include <png.h>

void abort_(const char * s, ...)
{
        va_list args;
        va_start(args, s);
        vfprintf(stderr, s, args);
        fprintf(stderr, "\n");
        va_end(args);
        abort();
}

int PngImage::OpenImage(const char* file_name)
{
    char header[8];    // 8 is the maximum size that can be checked

    /* open file and test for it being a png */
    FILE *fp = fopen(file_name, "rb");
    if (!fp)
	{
            std::cout <<"[read_png_file] File %s could not be opened for reading" << file_name << std::endl;
			return -1;
	}
    fread(header, 1, 8, fp);

    /* initialize stuff */
    png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr)
	{
            std::cout <<"[read_png_file] png_create_read_struct failed" << std::endl;
			return -1;
	}

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
	{
            std::cout <<"[read_png_file] png_create_info_struct failed" << std::endl;
			return -1;
	}

    if (setjmp(png_jmpbuf(png_ptr)))
	{
            std::cout << "[read_png_file] Error during init_io" << std::endl;
			return -1;
	}

    png_init_io(png_ptr, fp);
    png_set_sig_bytes(png_ptr, 8);

    png_read_info(png_ptr, info_ptr);

    width = png_get_image_width(png_ptr, info_ptr);
    height = png_get_image_height(png_ptr, info_ptr);
    color_type = png_get_color_type(png_ptr, info_ptr);

	if(color_type != 2)
	{
		std::cout << "we dont yet support other color types than RGB!" << std::endl;
		return -1;
	}
	else
		std::cout << "color type "<< int(color_type) << std::endl;

    bit_depth = png_get_bit_depth(png_ptr, info_ptr);
	std::cout << "bit depth "<< int(bit_depth) << std::endl;


    number_of_passes = png_set_interlace_handling(png_ptr);
	std::cout << "passes "<< int(number_of_passes) << std::endl;
    png_read_update_info(png_ptr, info_ptr);


    /* read file */
    if (setjmp(png_jmpbuf(png_ptr)))
	{
            std::cout << "[read_png_file] Error during read_image" << std::endl;
			return -1;
	}

    row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
    for (y=0; y<height; y++)
            row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));

    png_read_image(png_ptr, row_pointers);

    fclose(fp);
	return 0;
}

void PngImage::SavePngFile(char* file_name)
{
    /* create file */
    FILE *fp = fopen("anotherImage.png", "wb");
    if (!fp)
            abort_("[write_png_file] File %s could not be opened for writing", file_name);


    /* initialize stuff */
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr)
            abort_("[write_png_file] png_create_write_struct failed");

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr)
            abort_("[write_png_file] png_create_info_struct failed");

    if (setjmp(png_jmpbuf(png_ptr)))
            abort_("[write_png_file] Error during init_io");

    png_init_io(png_ptr, fp);


    /* write header */
    if (setjmp(png_jmpbuf(png_ptr)))
            abort_("[write_png_file] Error during writing header");

    png_set_IHDR(png_ptr, info_ptr, width, height,
                 bit_depth, color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);


    /* write bytes */
    if (setjmp(png_jmpbuf(png_ptr)))
            abort_("[write_png_file] Error during writing bytes");

    png_write_image(png_ptr, row_pointers);


    /* end write */
    if (setjmp(png_jmpbuf(png_ptr)))
            abort_("[write_png_file] Error during end of write");

    png_write_end(png_ptr, NULL);

    /* cleanup heap allocation */
    for (y=0; y<height; y++)
            free(row_pointers[y]);
    free(row_pointers);

    fclose(fp);
}


void PngImage::process_file(void)
{
	std::cout << "process file" << std::endl;
        if (png_get_color_type(png_ptr, info_ptr) == PNG_COLOR_TYPE_RGB)
                abort_("[process_file] input file is PNG_COLOR_TYPE_RGB but must be PNG_COLOR_TYPE_RGBA "
                       "(lacks the alpha channel)");

        if (png_get_color_type(png_ptr, info_ptr) != PNG_COLOR_TYPE_RGBA)
                abort_("[process_file] color_type of input file must be PNG_COLOR_TYPE_RGBA (%d) (is %d)",
                       PNG_COLOR_TYPE_RGBA, png_get_color_type(png_ptr, info_ptr));

        for (y=0; y<height; y++) {
                png_byte* row = row_pointers[y];
                for (x=0; x<width; x++) {
                        png_byte* ptr = &(row[x*4]);
                        //printf("Pixel at position [ %d - %d ] has RGBA values: %d - %d - %d - %d\n",
                        //       x, y, ptr[0], ptr[1], ptr[2], ptr[3]);

                        /* set red value to 0 and green value to the blue one */
                        ptr[0] = 0;
                        ptr[1] = ptr[2];
                }
        }
}