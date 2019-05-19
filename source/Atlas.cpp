#include "Atlas.h"
#include <iostream>

void Atlas::SaveAtlas(char* filename, char* title, std::vector<PngImage *> images)
{
   int code = 0;
   FILE *fp = NULL;
   png_structp png_ptr = NULL;
   png_infop info_ptr = NULL;
   png_bytep row = NULL;

   // Open file for writing (binary mode)
   fp = fopen(filename, "wb");
   if (fp == NULL) {
      fprintf(stderr, "Could not open file %s for writing\n", filename);
      code = 1;
	  goto finalise;
   }

   // Initialize write structure
   png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
   if (png_ptr == NULL) {
      fprintf(stderr, "Could not allocate write struct\n");
      code = 1;
      goto finalise;
   }

   // Initialize info structure
   info_ptr = png_create_info_struct(png_ptr);
   if (info_ptr == NULL) {
      fprintf(stderr, "Could not allocate info struct\n");
      code = 1;
      goto finalise;
   }

   // Setup Exception handling
   if (setjmp(png_jmpbuf(png_ptr))) {
      fprintf(stderr, "Error during png creation\n");
      code = 1;
      goto finalise;
   }

   int xPos = 0, yPos = 0;
   width = 0;
   height = 0;

   for(int i=0; i<images.size(); ++i)
   {
	   PngImage* img = images[i];
	   std::cout<< img->width << std::endl;
	   width += img->width;
	   height += img->height;
   }

   png_init_io(png_ptr, fp);

   // Write header (8 bit colour depth)
   png_set_IHDR(png_ptr, info_ptr, width, height,
         8, PNG_COLOR_TYPE_RGB, PNG_INTERLACE_NONE,
         PNG_COMPRESSION_TYPE_BASE, PNG_FILTER_TYPE_BASE);

   // Set title
   if (title != NULL) {
      png_text title_text;
      title_text.compression = PNG_TEXT_COMPRESSION_NONE;
      title_text.key = "Title";
      title_text.text = title;
      png_set_text(png_ptr, info_ptr, &title_text, 1);
   }

   png_write_info(png_ptr, info_ptr);

   // Allocate memory for one row (3 bytes per pixel - RGB)
   row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
    for (y=0; y<height; y++)
            row_pointers[y] = (png_byte*) malloc(3 * width * sizeof(png_byte));

   int x, y;
   /*for (y=0 ; y<height ; y++) 
   {
		//png_byte* rowC = copyThis->row_pointers[y];

		for (x=0 ; x<width ; x++) {
			png_byte* ptr = &(row[x*3]);

			//png_byte* ptrC = &(rowC[x*3]);
		
			ptr[0] = 255;
			ptr[1] = 255;
			ptr[2] = 255;
		}

      png_write_row(png_ptr, row);
   }*/

   //write images to the atlas
   for(int i=0; i<images.size(); ++i)
   {
	   PngImage* img = images[i];
	   for (y = 0 ; y < img->height ; y++) 
		{
			png_byte* rowCopy = img->row_pointers[y];
			png_byte* row = row_pointers[y];

			for (x = 0 ; x < img->width ; x++) {
				png_byte* ptr = &(row[(x+xPos)*3]);

				png_byte* ptrCopy = &(rowCopy[x*3]);
			
				ptr[0] = ptrCopy[0];
				ptr[1] = ptrCopy[1];
				ptr[2] = ptrCopy[2];
			}
		}
	   xPos += img->width;
   }

   png_write_image(png_ptr, row_pointers);

   // End write
   png_write_end(png_ptr, NULL);

   finalise:
   if (fp != NULL) fclose(fp);
   if (info_ptr != NULL) png_free_data(png_ptr, info_ptr, PNG_FREE_ALL, -1);
   if (png_ptr != NULL) png_destroy_write_struct(&png_ptr, (png_infopp)NULL);
   if (row != NULL) free(row);
}
