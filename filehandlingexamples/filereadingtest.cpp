#include <iostream>
#include <png.h>  // Libpng
#include <stdio.h>  //Used for FILE, fopen
using namespace std;

//COPIED CODE WE NEED TO WRITE OUR OWN (zarb.org/~gc/html/libpng.html) [source]
//There is no error handling in this code, as it is an example and for instructional use.
bool isPNG = false;
bool readTrue = true;
int width, height;
png_byte color_type;
png_byte bit_depth;
png_bytep * row_pointers;

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
  png_structp png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);  /*Initializing png_struct, one of the structures we need.*/
  png_infop info_ptr = png_create_info_struct(png_ptr);  /*Initializing png_info, which is the second structure.*/
  png_init_io(png_ptr, x);  //Passes file pointer to function that sets up input code.  Needs to use fread.
  png_set_sig_bytes(png_ptr, 8);  //png_sig_cmp took bytes from the start of the file, this gives them back to the file.
  //png_set_user_limits(png_ptr, width_max, heigth_max);  //Used to set limits on png input size.  It defaults to 1 million by 1 million.
  png_read_info(png_ptr, info_ptr);  //Reads all file information except actual image data.
  width = png_get_image_width(png_ptr, info_ptr);  //Gets image width
  height = png_get_image_height(png_ptr, info_ptr);  //Gets image height
  color_type = png_get_color_type(png_ptr, info_ptr);  //Color type is the type of color.  (ex. RGB, greyscale, etc.)
  bit_depth = png_get_bit_depth(png_ptr, info_ptr);  /*Bit depth is the amount of bits needed to contain information
  about 1 pixel of an image.  RGB is normall 8.*/
  if(width != 0 && height != 0){  //prints image width and height.
    cout << "Width is " << width << "!" << endl;
    cout << "Height is " << height << "!" << endl;
  }

  //READING IT FINALLY OMG
  row_pointers = new png_bytep [height];  //Initializing 2d array of type png_byte.  Image will be put in here.
  for (int i = 0; i < height; i++){
    row_pointers[i] = new png_byte [width];  //Technically this could be a memory leak.  Make sure to delete this when appropriate.
  }

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


int main(){
  char file[100];  //file name is put in here.  Must be char * because libpng is in c.
  cout << "Gimme dat file name" << endl;
  cin >> file;
  read_png_file(file);
  return 0;
}
