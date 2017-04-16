#include <iostream>
#include <cstdlib>
#include <cmath>
#include <cstdio>
#include "Picture.h"

using namespace std;

Picture::Picture(char *fileName){
	readPNGFile(fileName);
}

/* Function to open up a PNG picture file and read in its information. This method takes in a character array as input to represent the name of the file that is to be
opened (the input must be a character array since the libpng library is written in C and cannot handle strings). The function contains no output. It simply opens the PNG
file, and creates a Picture object based on the information inside the file. */
void Picture::readPNGFile(char *filename){
  
  // Open a file based on the name passed in
  fp = fopen(filename, "rb");
  if(!fp){
    cout << "Error, invalid file type" << endl;
    return;
  }
  
  // Using the libpng library - two structures need to be initalized to read in the information of a PNG picture
  // Initalize the first struct to read in the PNG file  
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if(!png_ptr) {
    cout << "Error, unable to make png_ptr, exiting" << endl;
    return;
  }
  
  // Initalize the second structure to contain the information from the PNG picture
  info_ptr = png_create_info_struct(png_ptr);
  if(!info_ptr){
    cout << "Error, unable to make info_ptr, exiting" << endl;  
  }

  if(setjmp(png_jmpbuf(png_ptr))){
    return;
  }

  png_init_io(png_ptr, fp);// Passes file pointer to function that sets up input code.

  png_read_info(png_ptr, info_ptr);// Function to read in information regarding the picture (but it does not actually read in the raw picture data)
  
  // Once the picture information has been read into the program - a Picture object can be established based on the information from the picture
  width      = png_get_image_width(png_ptr, info_ptr); // Extract the width of the picture
  height     = png_get_image_height(png_ptr, info_ptr); // Extract the height of the picture
  color_type = png_get_color_type(png_ptr, info_ptr); // Extact the color type (e.g. grayscale, RGB...)
  bit_depth  = png_get_bit_depth(png_ptr, info_ptr); // Extact bit depth (the amount of bits needed to contain information for each pixel. RGB is normally 8)
  
	/* Perform several checks to ensure that the picture information is of RGB format */ 
 
	// Check if the bit depth needs to be altered so that it is compatable with RGB
  if(bit_depth == 16)
    png_set_strip_16(png_ptr);
    
	// Check if the color type needs to be altered from pallette to RGB
  if(color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_palette_to_rgb(png_ptr);

	// Check if color type is of grayscale format and has less than 8 bits - in which case, the bit count needs to be expanded. GRAY has eithet 8 or 16 bits.
  if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    png_set_expand_gray_1_2_4_to_8(png_ptr);

	// Check if the image contains transparency information - which case, an alpha channel needs to be added
  if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
    png_set_tRNS_to_alpha(png_ptr);

  // Check if the picture is of grayscale format - in which case, it needs to be adjusted to RGB format  
  if(color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    png_set_gray_to_rgb(png_ptr);

	// Check if no alpha channel exists - in which case, it needs to be filled in
   if(color_type == PNG_COLOR_TYPE_RGB ||
     color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_PALETTE)
    png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
  
  // Update picture structs now that all checks have been performed to ensure that the picture is in RGB format
  png_read_update_info(png_ptr, info_ptr);


  /* Officially begin to read in the raw picture data from the file */
  // Allocate memory for the picture data based on its size
  // Dynamically establish a two-dimensional array to hold each pixel in the file
  row_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
  for(int y = 0; y < height; y++) {
    row_pointers[y] = (png_byte*)malloc(png_get_rowbytes(png_ptr,info_ptr));   // Read in the pixel information from the picture into the memory allocated for it
  }

  png_read_image(png_ptr, row_pointers);

  fclose(fp); //Close the file.
}


/* This function opens up a file and writes a PNG file to it. The method takes in a character array as input that will be used to represent the file name (a character
array must be used instead of a string because libpng is written in C and cannot function with strings). The function contains no ouutput. */
void Picture::writePNGFile(char *filename) {
  
	// Open up a new file that will be used to write a PNG picture to
  FILE *fp = fopen(filename, "wb");
  if(!fp){
    cout << "Error, invalid file type" << endl;
    return;
  }

	/*Two structures need to be created as part of the libpng library. One structure will be used to hold the information from the picture and the 
	other structure will be used to write the information to the file.*/

	// Establish the struct for writing the PNG image
  png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (!png_ptr){
    cout << "Error, png_ptr not made, exiting" << endl;
    return;
  }

	// Establish the struct for handling the information of the PNG image
  png_infop info_ptr = png_create_info_struct(png_ptr);
  if (!info_ptr){
    cout << "Error, info_ptr not made, exiting" << endl;
  }

  if (setjmp(png_jmpbuf(png_ptr))){
    return;
  }

  png_init_io(png_ptr, fp); // Initializes file to be written to


  // // Set basic parameters for the image. Output is going to be 8 bits length and RGB.
  png_set_IHDR(
    png_ptr,
    info_ptr,
    width, height,
    8,
    PNG_COLOR_TYPE_RGBA,
    PNG_INTERLACE_NONE,
    PNG_COMPRESSION_TYPE_DEFAULT,
    PNG_FILTER_TYPE_DEFAULT
  );
  png_write_info(png_ptr, info_ptr);// Write picture information to the file (excluding the pixel data)

  png_write_image(png_ptr, row_pointers);// Write picture pixel data to the file
  png_write_end(png_ptr, NULL);// Finishes writing to the end of the file

  //deletes 2d array of pointers so there is no memory leaking.
  for(int y = 0; y < height; y++) {
    free(row_pointers[y]);
  }
  free(row_pointers);

  fclose(fp);//Closes file.
}

//look up conversion from rgb to hsv for exact formula
void Picture::convertToHSV(int r, int g, int b, double &h, double &s, double &v){
  double rP = (double)r/255.0;
  double gP = (double)g/255.0;
  double bP = (double)b/255.0;
  double max = -1.0;//smallest number to start with
  double min = 400.0;//outrageously large number initially
  double hue = 0.0;
  double sat = 0.0;
  double val = 0.0;
  
  double RGB [3] = {rP, gP, bP};
  
  //convert to 8-bit color depth
  int maxInd;
  for (int i=0;i< 3;i++){
    if (RGB[i] < min)
      min = RGB[i];
      
    if (RGB[i] > max){
      max = RGB[i];
      maxInd = i;
    }
  }
  
  double delta = max - min;
  
  if(delta < 0.000001){
    hue = 0.0;
  }
  else if(maxInd == 0){
    double mod_factor = (gP-bP)/delta;
    while(mod_factor <= 0.0){
      mod_factor = mod_factor += 6.0;
    }
    while(mod_factor >= 6.0){
      mod_factor = mod_factor - 6.0;
    }
  
    hue = mod_factor;
  }
  
  else if(maxInd == 1){
    hue = (bP - rP)/delta + 2.0;
  }
  else if(maxInd == 2){
    hue = (rP - gP)/delta + 4.0;
  }
  else{
    cout << "error" << endl;
  }
  
  hue = hue * 60.0;
  
  if(max < 0.001){
    sat = 0.0;
  }
  
  else{
    sat = delta/max;
  }
  
  val = max;
  
  h = hue;
  s = sat;
  v = val;
  
}

//look up conversion from hsv to rgb for exact formula
void Picture::convertToRGB(double hue, double sat, double val, int &r, int &g, int &b){
  double c = val * sat;
  double mod_factor = hue / 60.0;
  while(mod_factor >= 2.0)
    mod_factor = mod_factor - 2.0;
  while(mod_factor <= 0.0)
    mod_factor = mod_factor + 2.0;
  double x = c * (1 - abs(mod_factor - 1));
  double m = val - c;
  
  double rP, gP, bP;
  if((0.0 <= hue && hue < 60.0)){
      rP = c;
      gP = x;
      bP = 0;
  }
  else if(60.0 <= hue && hue < 120.0){
      rP = x;
      gP = c;
      bP = 0;
  }
  else if(120.0 <= hue && hue < 180.0){
      rP = 0;
      gP = c;
      bP = x;
  }
  else if(180.0 <= hue && hue < 240.0){
      rP = 0;
      gP = x;
      bP = c;
  }
  else if(240.0 <= hue && hue < 300.0){
      rP = x;
      gP = 0;
      bP = c;
  }
  else if(300.0 <= hue && hue < 360.0){
      rP = c;
      gP = 0;
      bP = x;
  }
  
  r = (int)((rP + m) * 255.0);
  g = (int)((gP + m) * 255.0);
  b = (int)((bP + m) * 255.0);
}


void Picture::hue(int value){
  //restricts input to a closed interval (slider)
  if(value > 360 || value < -359){
    //cout << "INVALID" << endl;
    return;
  }
  int r, g, b;
  double h, s, v;
  for (int y = 0; y < height; y++) {
    png_byte* row = row_pointers[y];
    for (int x = 0; x < width; x++) {
      png_byte* ptr = &(row[x*4]);
      r = ptr[0];
      g = ptr[1];
      b = ptr[2];
      convertToHSV(r, g, b, h, s, v);
      h = h + (double)value;
      //fixes the max and min values of the possible hue. anything that may go over is stopped exactly at the max.
      if(h > 360.0)
        h = 359.0;
      if(h < 0)
        h = 0;
      convertToRGB(h, s, v, r, g, b);
      ptr[0] = r;
      ptr[1] = g;
      ptr[2] = b;
    }
  }
}

//Can only go from 0 to 1, the value is a percentage so must be inbetween -1 and 1.
void Picture::saturation(int value) {
  if(value > 100 || value < -100){
    cout << "INVALID" << endl;
    return;
  }
  int r, g, b;
  double h, s, v;
  for (int y = 0; y < height; y++) {
    png_byte* row = row_pointers[y];
    for (int x = 0; x < width; x++) {
      png_byte* ptr = &(row[x*4]);
      r = ptr[0];
      g = ptr[1];
      b = ptr[2];
      convertToHSV(r, g, b, h, s, v);
      s = s + (double)value/100.0;
      //anything set over the possible max is made to be exactly at the min/max, which in this case is 0 and 1.
      if(s > 1.0)
        s = 1.0;
      if(s < 0)
        s = 0;
      convertToRGB(h, s, v, r, g, b);
      ptr[0] = r;
      ptr[1] = g;
      ptr[2] = b;
    }
  }
}


//can read in either a double directly or take in an int and process it inside, makes no difference
void Picture::brightness(int value){
  if(value > 100 || value < -100){
  	//WILL MOST LIKELY HAVE TO TAKE OUT 
    //cout << "INVALID" << endl;
    return;
  }
  int r, g, b;
  double h, s, v;
  for (int y = 0; y < height; y++) {
    png_byte* row = row_pointers[y];
    for (int x = 0; x < width; x++) {
      png_byte* ptr = &(row[x*4]);
      r = ptr[0];
      g = ptr[1];
      b = ptr[2];
      convertToHSV(r, g, b, h, s, v);
      v = v + ((double)value/100.0);
      //anything set over the possible max is made to be exactly at the max instead of over/under the max.
      if(v > 1.0)
        v = 1.0;
      if(v <= 0.0)
        v = 0.0;
      convertToRGB(h, s, v, r, g, b);
      ptr[0] = r;
      ptr[1] = g;
      ptr[2] = b;
    }
  }
	
}


//this method will adjust the contrast
//int value is to be between the values of -255 to 255. "-" reduces contrast,
// "+" increases it
void Picture::contrast(int value){
  if(value < -255 || value > 255){
    //cout << "Error" << endl;
    return;
  }

  /*
  factor = (259 * (contrast + 255)) / (255 * (259 - contrast))
colour = GetPixelColour(x, y)
newRed   = Truncate(factor * (Red(colour)   - 128) + 128)
newGreen = Truncate(factor * (Green(colour) - 128) + 128)
newBlue  = Truncate(factor * (Blue(colour)  - 128) + 128)
PutPixelColour(x, y) = RGB(newRed, newGreen, newBlue)

  
  */
  
  
  double cFactor = (259.0*((double)value + 255.0)) / (255.0*(259.0 - (double)value));
  int r, g, b;
  for (int y = 0; y < height; y++) {
    png_byte* row = row_pointers[y];
    for (int x = 0; x < width; x++) {
      png_byte* ptr = &(row[x*4]);
      r = ptr[0];
      g = ptr[1];
      b = ptr[2];
      
      r = (int)(cFactor*(r - 128.0) + 128);
      g = (int)(cFactor*(g - 128.0) + 128);
      b = (int)(cFactor*(b - 128.0) + 128);
      
      ptr[0] = clamp(r);
      ptr[1] = clamp(g);
      ptr[2] = clamp(b);
      
    }
  }
}


int Picture::clamp(int p){
  if(p < 0)
    return 0;
  else if(p > 255)
    return 255;
  return p;
}

void Picture::temperature(int value){
  if(value > 255 || value < -255){
    //cout << "Error" << endl;
    return;
  }
  int r, g, b;
  for (int y = 0; y < height; y++) {
    png_byte* row = row_pointers[y];
    for (int x = 0; x < width; x++) {
      png_byte* ptr = &(row[x*4]);
      ptr[0] = clamp(ptr[0] + value);
      ptr[2] = clamp(ptr[2] - value);
    }
  }
}

void Picture::verticalFlip(){
  png_bytep temp;
  for(int y = 0; y < height/2; y++) {
    temp = row_pointers[y];
    row_pointers[y] = row_pointers[height-1-y];
    row_pointers[height-1-y] = temp;
  }
}

void Picture::rotateLeft(){
  //Using a temporary variable to swap the width and height values, as new dimensions must be made
  int temp;
  temp = width;
  width = height;
  height = temp;
  
  //Creates a new array of png_bytes to replace previous, using the new hieght and widths
  png_bytep * copy_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
  for(int i = 0; i < height; i++){
    copy_pointers[i] = (png_byte*)malloc(width * 4);//The malloc in the read method is essentially this
  }
  
  
  for(int y = 0; y < height; y++) {
    png_bytep copy = copy_pointers[y];
    for(int x = 0; x < width; x++) {
      png_bytep row = row_pointers[x];
      png_bytep px = &(row[y*4]);
      png_bytep pc = &(copy[x*4]);
      pc[0] = px[0];
      pc[1] = px[1];
      pc[2] = px[2];
      pc[3] = px[3];
    }   
  }
  
  for(int y = 0; y < width; y++) {
    free(row_pointers[y]);
  }
  free(row_pointers);
  
  row_pointers = copy_pointers;
  
  verticalFlip();
}

void Picture::rotateRight(){
  //reversing dimensions of newly oriented image
  int temp;
  temp = width;
  width = height;
  height = temp;
  
  //makes a copy of the png_byte array to store rotated data
  png_bytep * copy_pointers = (png_bytep*)malloc(sizeof(png_bytep) * height);
  for(int i = 0; i < height; i++){
    copy_pointers[i] = (png_byte*)malloc(width * 4);
  }
  
  
  for(int y = height-1; y >= 0; y--) {
    png_bytep copy = copy_pointers[y];
    for(int x = width-1; x >= 0; x--) {
      png_bytep row = row_pointers[width-x-1];
      png_bytep px = &(row[(height-1-y)*4]);
      png_bytep pc = &(copy[x*4]);
      pc[0] = px[0];
      pc[1] = px[1];
      pc[2] = px[2];
      pc[3] = px[3];
    }   
  }
  
  for(int y = 0; y < width; y++) {
    free(row_pointers[y]);
  }
  free(row_pointers);
  
  row_pointers = copy_pointers;
  
  verticalFlip();  
}

void Picture::horizontalFlip(){
  rotateLeft();
  rotateLeft();
  verticalFlip();
}

int main(){
  char fileName[100];  //file name is put in here.  Must be char * because libpng is in c.
  cout << "Please enter a file name to open: ";
  cin >> fileName;
  Picture test(fileName);
  
  //Do stuff here
  test.contrast(128);
  //etc.
  
  //Writes to output file
  cout << "Please enter a file name to save the file: ";
  cin >> fileName;
  test.writePNGFile(fileName);
  return 0;
}
