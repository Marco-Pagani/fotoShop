#include <iostream>
#include <png.h>  // Libpng
#include <stdio.h>  //Used for FILE, fopen
#include <cstdlib>
using namespace std;

//IMPORTANT NOTE: THIS PROGRAM CURRENTLY COMPLETELY MESSES UP THE IMAGE.  BUT AT LEAST IT READS AND WRITES AN IMAGE.

//COPIED CODE WE NEED TO WRITE OUR OWN (zarb.org/~gc/html/libpng.html) [source]
//There is no error handling in this code, as it is an example and for instructional use.
bool isPNG = false;
bool readTrue = true;
int width, height;
int number_of_passes;
png_byte color_type;
png_byte bit_depth;
png_bytep * row_pointers;
png_structp png_ptr;
png_infop info_ptr;

void read_png_file(char * file_name){  //must take char * because libpng is written in c,not c++.

  //Open file and test for png.
  png_byte header[8];  //This will take the first 8 bytes of the file.  Libpng uses the first 8 bytes to determine if a file is a png.
  FILE * x = fopen(file_name, "rb");  //r opens a file for reading, b opens it in binary.
  fread(header, 1, 8, x); //fread(pointer to put stuff in, how many bytes per element, how many elements total, file pointer)
  if(!png_sig_cmp(header, 0, 8)){ //png_sig_cmp returns false if file is png, true otherwise.  8 = number of bytes read, I dunno what 0 is but I think it will always be 0.
    isPNG = true;  //for verification purposes.
    cout << "png_sig_cmp worked" << endl;  //This is for bug handling.
  }

  //Initialization
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);  /*Initializing png_struct, one of the structures we need.*/
  if(!png_ptr){  //checks to make sure png_struct was created.
    cout << "png_struct not made" << endl;
    return;
  }
  info_ptr = png_create_info_struct(png_ptr);  /*Initializing png_info, which is the second structure.*/
  png_init_io(png_ptr, x);  //Passes file pointer to function that sets up input code.  Needs to use fread.
  png_set_sig_bytes(png_ptr, 8);  //png_sig_cmp took bytes from the start of the file, this gives them back to the file.
  //png_set_user_limits(png_ptr, width_max, heigth_max);  //Used to set limits on png input size.  It defaults to 1 million by 1 million.
  png_read_info(png_ptr, info_ptr);  //Reads all file information except actual image data.
  // if(color_type == PNG_COLOR_TYPE_PALETTE){  //Expands paletted images to RGB?
  //   png_set_palette_to_rgb(png_ptr);
  // }
  width = png_get_image_width(png_ptr, info_ptr);  //Gets image width
  height = png_get_image_height(png_ptr, info_ptr);  //Gets image height
  color_type = png_get_color_type(png_ptr, info_ptr);  //Color type is the type of color.  (ex. RGB, greyscale, etc.)
  bit_depth = png_get_bit_depth(png_ptr, info_ptr);  /*Bit depth is the amount of bits needed to contain information
  about 1 pixel of an image.  RGB is normally 8.*/

  //Delete if this don't work.
  // number_of_passes = png_set_interlace_handling(png_ptr);
  // png_read_update_info(png_ptr, info_ptr);

  if(width != 0 && height != 0){  //prints image width and height.
    cout << "Width is " << width << "!" << endl;
    cout << "Height is " << height << "!" << endl;
  }

  //READING IT FINALLY OMG
  row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
  for (int y=0; y<height; y++)
          row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));

  try{
    png_read_image(png_ptr, row_pointers);  //Actually reading the image.
  }catch(...){
    cout << "png_read_image did not work" << endl;
    readTrue = false;
  }

  if(readTrue){
    cout << "png_read_image worked" << endl;
  }

  fclose(x);
}

void write_png_file(char * outputFile){
  //http://www.libpng.org/pub/png/book/chapter15.html was of especial use.
  FILE * output = fopen(outputFile, "wb");  //Creates new file to write to.
  png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);  //Creates png and info struct for writing.
  info_ptr = png_create_info_struct(png_ptr);
  png_init_io(png_ptr, output);  //Initializes output.
  png_set_IHDR(png_ptr, info_ptr, width, height, bit_depth, color_type, PNG_INTERLACE_NONE, PNG_COMPRESSION_TYPE_DEFAULT, PNG_FILTER_TYPE_DEFAULT);  //Sets basic image parameters.
  png_write_info(png_ptr, info_ptr);  //Writes all non-image data to file.
  cout << "got up to png_write_image" << endl;
  png_write_image(png_ptr, row_pointers);  //Writes image data to file.
  cout << "got up to png_write_end" << endl;
  png_write_end(png_ptr, NULL); //Finishes writing file.  Change NULL to the info pointer if you want comments or time.
  cout << "got up to deleting array" << endl;
  // for(int i = 0; i < height; i++){  //Deletes 2d array so no memory leak.  This currently causes the program to crash.
  //   delete [] row_pointers[i];
  // }
  // delete [] row_pointers;
  cout << "got up to fclose()" << endl;
  fclose(output);

  cout << "well, it didn't crash" << endl;

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
