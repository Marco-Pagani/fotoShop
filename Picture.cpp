#include <iostream>
#include <stdio.h>
#include <png.h>  // libpng library - used for file handling of a png file
#include "Picture.h"

using namespace std;

/* Function to open up a PNG picture file and read in its information. This method takes in a character array as input to represent the name of the file that is to be
opened (the input must be a character array since the libpng library is written in C and cannot handle strings). The function contains no output. It simply opens the PNG
file, and creates a Picture object based on the information inside the file. */
void read_png_file(char * fileName)
{  
  // Create an array to read in the first 8 bits of a png file. The first 8 bits represents the header portion for a PNG picture.
  png_byte pngHeader[8];  

  // Open a file based on the name passed in
  FILE *file = fopen(fileName, "rb");

  // Read in the header signature of the PNG file. 
  fread(pngHeader, 1, 8, file); 

  // Check if the file opened is indeed a PNG file
  /* png_sig_cmp is a library function that checks if a file opened is a PNG picture based on its header. 
  It returns 0 if the first 8 bits of the file indicate that the file is a PNG picture */
  if(png_sig_cmp(pngHeader, 0, 8))
  { 
    // ERROR - BREAK FROM CODE
    return;
  }

  // Using the libpng library - two structures need to be initalized to read in the information of a PNG picture

  // Initalize the first struct to read in the PNG file 
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);  
  
  // Check if the information structure has been properly created 
  if(!png_ptr) 
  {  
    // ERROR - BREAK FROM CODE
    return;
  }

  // Initalize the second structure to contain the information from the PNG picture
  info_ptr = png_create_info_struct(png_ptr); 

  png_init_io(png_ptr, file);  // Passes file pointer to function that sets up input code.

  png_set_sig_bytes(png_ptr, 8);  // Re-establishes the header portion of the pointer file. This is used to counter reading in the first 8 bits at the beginning

  png_read_info(png_ptr, info_ptr);  // Function to read in information regarding the picture (but it does not actually read in the raw picture data)

  // Now that the picture information has been read into the program - a Picture object can be established based on the information from the picture
  width = png_get_image_width(png_ptr, info_ptr);   // Extract the width of the picture
  height = png_get_image_height(png_ptr, info_ptr);  // Extract the height of the picture
  color_type = png_get_color_type(png_ptr, info_ptr);  // Extact the color type (e.g. grayscale, RGB...)
  bit_depth = png_get_bit_depth(png_ptr, info_ptr);  // Extact bit depth (the amount of bits needed to contain information for each pixel. RGB is normally 8)

 
  /* Officially begin to read in the raw picture data from the file */
  // Allocate memory for the picture data based on its size
  // Dynamically establish a two-dimensional array to hold each pixel in the file
  row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
  for (int y=0; y<height; y++)
          row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));

  try
  {
    png_read_image(png_ptr, row_pointers);   // Read in the pixel information from the picture into the memory allocated for it
  } catch(...)
  {
    cout << "png_read_image did not work" << endl;
    readTrue = false;
  }

  fclose(x);   // Close the file
}


/* This function opens up a file and writes a PNG file to it. The method takes in a character array as input that will be used to represent the file name (a character
array must be used instead of a string because libpng is written in C and cannot function with strings). The function contains no ouutput. */
void write_png_file(char *fileName) 
{
	// Open up a new file that will be used to write a PNG picture to
	FILE *file = fopen(fileName, "wb"); 

	/*Two structures need to be created as part of the libpng library. One structure will be used to hold the information from the picture and the 
	other structure will be used to write the information to the file.*/

	// Establish the struct for writing the PNG image
	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL); 
	// Establish the struct for handling the information of the PNG image
	info_ptr = png_create_info_struct(png_ptr);

	png_init_io(png_ptr, file);  // Initializes file to be written to

	// Set basic parameters for the image
	png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);  
	
	png_write_info(png_ptr, info_ptr);  // Write picture information to the file (excluding the pixel data)
	
	png_write_image(png_ptr, row_pointers);  // Write picture pixel data to the file
	
	png_write_end(png_ptr, NULL);   // Finishes writing to the end of the file

	// for(int i = 0; i < height; i++){  //Deletes 2d array so no memory leak.  This currently causes the program to crash.
	//   delete [] row_pointers[i];
	// }
	// delete [] row_pointers;


	fclose(file);   // Closes the file
}

int main(){
  char file[100];  //file name is put in here.  Must be char * because libpng is in c.
  cout << "Gimme dat file name" << endl;
  cin >> file;
  read_png_file(file);
  cout << "Gimme dat output file name." << endl;
  cin >> file;
  write_png_file(file);
  return 0;
}