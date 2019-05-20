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

PngImage::~PngImage()
{
	if(row_pointers == nullptr)
		return;

	for (int y=0; y<height; y++)
        delete[](row_pointers[y]);
    delete[](row_pointers);
}

int PngImage::OpenImage(const char* file_path, const char* file_name)
{
	if(row_pointers != nullptr)
		return -1;

    char header[8];    // 8 is the maximum size that can be checked

    /* open file and test for it being a png */
    FILE *fp = fopen(file_path, "rb");
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
	name = file_name;
	area = width*height;
    color_type = png_get_color_type(png_ptr, info_ptr);

	if(color_type != 2 && color_type != 6)
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

	row_pointers = new png_bytep[height];
    for (int y=0; y<height; y++)
		row_pointers[y] = new png_byte[width*4];
			

    png_read_image(png_ptr, row_pointers);

    fclose(fp);
	return 0;
}

void PngImage::SavePngFile(char* file_name)
{
    /* create file */
    FILE *fp = fopen(file_name, "wb");
    if (!fp){
		std::cout << "[write_png_file] File %s could not be opened for writing" << file_name << std::endl;
		return;
	}


    /* initialize stuff */
    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    if (!png_ptr){
		std::cout << "[write_png_file] png_create_write_struct failed" << std::endl;
		return;
	}

    info_ptr = png_create_info_struct(png_ptr);
    if (!info_ptr){
		std::cout << "[write_png_file] png_create_info_struct failed" << std::endl;
		return;
	}

    if (setjmp(png_jmpbuf(png_ptr))){
            std::cout << "[write_png_file] Error during init_io" << std::endl;
			return;
	}

    png_init_io(png_ptr, fp);


    /* write header */
    if (setjmp(png_jmpbuf(png_ptr))){
            std::cout << "[write_png_file] Error during writing header"<< std::endl;
			return;
	}

    png_set_IHDR(png_ptr, info_ptr, width, height,
                 bit_depth, color_type, PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

    png_write_info(png_ptr, info_ptr);


    /* write bytes */
    if (setjmp(png_jmpbuf(png_ptr))){
            std::cout <<"[write_png_file] Error during writing bytes"<< std::endl;
			return;
	}

    png_write_image(png_ptr, row_pointers);


    /* end write */
    if (setjmp(png_jmpbuf(png_ptr))){
            std::cout <<"[write_png_file] Error during end of write"<< std::endl;
			return;
	}

    png_write_end(png_ptr, NULL);

    fclose(fp);
	/*finalise:
	if (fp != NULL) fclose(fp);
	if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
	if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
	if (row != NULL) free(row);*/
}