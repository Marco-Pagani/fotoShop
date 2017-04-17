#include <iostream>
#include <stdio.h>
#include <cstdlib>
#include <png.h> // libpng library - used for file handling of a png file
#include <cmath>
#include "interface.h"

using namespace std;

/*  Function to open up a PNG picture file and read in its information. This met
 * hod takes in a character array as input to represent the name of the file tha
 * t is to be
opened (the input must be a character array since the libpng libra
 * ry is written in C and cannot handle strings). The function contains no outpu
 * t. It simply opens the PNG
file, and creates a Picture object based on the in
 * formation inside the file.
 */

void Interface::set_char(string file_name){
    filep = file_name.c_str();
}

void Interface::readPNGFile() {


  // Create an array to read in the first 8 bits of a png file. The first 8 bits represents the header portion for a PNG picture.
  //png_byte header[8];  //This will take the first 8 bytes of the file.  Libpng uses the first 8 bytes to determine if a file is a png.

  //Open a file based on the name passed in
  FILE * x = fopen(filep, "rb");  //r opens a file for reading, b opens it in binary.

  //Read in the header signature of the PNG file
  //fread(header, 1, 8, x); //fread(pointer to put stuff in, how many bytes per element, how many elements total, file pointer)



  // Check if the file opened is indeed a PNG file
  	/* png_sig_cmp is a library function that checks if a file opened is a PNG picture based on its header.
  	It returns 0 if the first 8 bits of the file indicate that the file is a PNG picture */
//  if(!png_sig_cmp(header, 0, 8)) //png_sig_cmp returns false if file is png, true otherwise.  8 = number of bytes read, I dunno what 0 is but I think it will always be 0.
//  {
//    isPNG = true;  //for verification purposes.
//    //ERROR - BREAK FROM CODE
//    cout << "png_sig_cmp worked" << endl;  //This is for bug handling.
//  }

  //Using the libpng library - two structures need to be initialized to read in the info of a PNG picture

  //Initialize the first struct to read in the PNG fule
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);  /*Initializing png_struct, one of the structures we need.*/

  //Check if the info structure has been properly created
  if(!png_ptr){  //checks to make sure png_struct was created.
    //ERROR - BREAK FROM CODE
    cout << "png_struct not made" << endl;
    return;
  }

  // Initalize the second structure to contain the information from the PNG picture
  info_ptr = png_create_info_struct(png_ptr);  /*Initializing png_info, which is the second structure.*/

  png_init_io(png_ptr, x);  //Passes file pointer to function that sets up input code.  Needs to use fread.

  //png_set_sig_bytes(png_ptr, 8);  //png_sig_cmp took bytes from the start of the file, this gives them back to the file.
  //png_set_user_limits(png_ptr, width_max, heigth_max);  //Used to set limits on png input size.  It defaults to 1 million by 1 million.

  png_read_info(png_ptr, info_ptr);  //Reads all file information except actual image data.
  // if(color_type == PNG_COLOR_TYPE_PALETTE){  //Expands paletted images to RGB?
  //   png_set_palette_to_rgb(png_ptr);
  // }


  // Once the picture information has been read into the program - a Picture object can be established based on the information from the picture
  width = png_get_image_width(png_ptr, info_ptr);  //Gets image width
  height = png_get_image_height(png_ptr, info_ptr);  //Gets image height
  color_type = png_get_color_type(png_ptr, info_ptr);  //Color type is the type of color.  (ex. RGB, greyscale, etc.)
  bit_depth = png_get_bit_depth(png_ptr, info_ptr);  /*Bit depth is the amount of bits needed to contain information
  about 1 pixel of an image.  RGB is normally 8.*/

  /* Perform several checks to ensure that the picture information is of RGB format */

  // Check if the bit depth needs to be altered so that it is compatable with RGB

  if(bit_depth == 16){ //Changes anything with bit depth of 16 to RGB.
    png_set_strip_16(png_ptr);
  }

  // Check if the color type needs to be altered from pallette to RGB
  if(color_type == PNG_COLOR_TYPE_PALETTE)  //Changes color_type from palette to rgb.
   png_set_palette_to_rgb(png_ptr);

  // Check if color type is of grayscale format and has less than 8 bits - in which case, the bit count needs to be expanded
  if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8){  //Changes grayscale images with less than 8 bits to 8 bits.
   png_set_expand_gray_1_2_4_to_8(png_ptr);
  }


  // Check if the image contains transparency information - which case, an alpha channel needs to be added
  if(png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)){  //If image has transparency info, adds alpha channel.
    png_set_tRNS_to_alpha(png_ptr);
  }


  if(color_type == PNG_COLOR_TYPE_RGB ||
     color_type == PNG_COLOR_TYPE_GRAY ||
     color_type == PNG_COLOR_TYPE_PALETTE){
       png_set_filler(png_ptr, 0xFF, PNG_FILLER_AFTER);
  }


  // Check if the picture is of grayscale format - in which case, it needs to be adjusted to RGB format
  if(color_type == PNG_COLOR_TYPE_GRAY || color_type == PNG_COLOR_TYPE_GRAY_ALPHA){
    png_set_gray_to_rgb(png_ptr);
  }

  // Update picture structs now that all checks have been performed to ensure that the picture is in RGB format
  png_read_update_info(png_ptr, info_ptr);


  if(width != 0 && height != 0){  //prints image width and height.
    cout << "Width is " << width << "!" << endl;
    cout << "Height is " << height << "!" << endl;
  }


  /* Officially begin to read in the raw picture data from the file */
  	// Allocate memory for the picture data based on its size
  	// Dynamically establish a two-dimensional array to hold each pixel in the file
  //READING IT FINALLY OMG
  row_pointers = (png_bytep*) malloc(sizeof(png_bytep) * height);
  for (int y=0; y<height; y++)
          row_pointers[y] = (png_byte*) malloc(png_get_rowbytes(png_ptr,info_ptr));

  try{
    png_read_image(png_ptr, row_pointers);  //Actually reading the image.
  }catch(...){
    cout << "png_read_image did not work" << endl;
  }

  fclose(x);
}

/* This function opens up a file and writes a PNG file to it. The method takes in a character array as input that will be used to represent the file name (a
character array must be used instead of a string because libpng is written in C and cannot function with strings). The function contains no output.*/
void Interface::writePNGFile() {

    // Open up a new file that will be used to write a PNG picture to
    //http://www.libpng.org/pub/png/book/chapter15.html was of especial use.
    FILE * output = fopen(filep, "wb"); //Creates new file to write to.

	/*Two structures need to be created as part of the libpng library. One structure will be used to hold the information from the picture and the
	other structure will be used to write the information to the file.*/

	// Establish the struct for writing the PNG image


    png_ptr       = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL); //Creates png and info struct for writing.

    // Establish the struct for handling the information of the PNG image
    info_ptr      = png_create_info_struct(png_ptr);


    png_init_io(png_ptr, output); //Initializes output.

    // Set basic parameters for the image
    png_set_IHDR(
        png_ptr,
        info_ptr,
        width,
        height,
        8,
        PNG_COLOR_TYPE_RGBA,
        PNG_INTERLACE_NONE,
        PNG_COMPRESSION_TYPE_DEFAULT,
        PNG_FILTER_TYPE_DEFAULT
    ); //Sets basic image parameters.

    png_write_info(png_ptr, info_ptr); //Writes all non-image data to file.

    png_write_image(png_ptr, row_pointers); //Writes image data to file.

    png_write_end(png_ptr, NULL); //Finishes writing file.  Change NULL to the info pointer if you want comments or time.

    // for(int i = 0; i < height; i++){  Deletes 2d array so no memory leak.  This
    // currently causes the program to crash.   delete [] row_pointers[i]; } delete
    // [] row_pointers;

    fclose(output);   // Close the file
}


/* Function for converting the picture information from a red-green-blue format to a hue-saturation-value format. The method takes in three integers to represent
the red, blue, and green value from the picture. It contains references to three doubles so that the address for hue, saturation, and value can be adjusted. The
function has no output after performing the conversion. */
void Interface::convertToHSV(int r, int g, int b, double &h, double &s, double &v)
{
	// Convert the red, blue, and green values into decimals
    double rP = (double)r/255.0;
    double gP = (double)g/255.0;
  	double bP = (double)b/255.0;

  	// Initalize values for min and max variable that will be used later in the conversion formula
  	double max = -1.0;   // The maximum value can never be smaller than -1
  	double min = 400.0;   // The minimum should never be higher than 400

  	// Initalize variables to represent the hue, saturation, and value
  	double hue = 0.0;
  	double sat = 0.0;
  	double val = 0.0;

  	// Create an array to hold the red, green, and blue values from the pixel
  	double RGB [3] = {rP, gP, bP};

  	// Convert each red, green, blue value to have 8-bit color depth
  	int maxInd;   // Initalize a variable to keep track of the index for which value is the largest between red, green, or blue
  	for (int i=0; i < 3; i++)
  	{
  		// Check which red, green, and blue value is the smallest of the three
    	if (RGB[i] < min)
      		min = RGB[i];

      	// Check which red, green, and blue value is the largest of the three
    	if (RGB[i] > max)
    	{
      		max = RGB[i];
      		maxInd = i;   // Update the value of the index that contains the largest value
    	}
  	}

  	double delta = max - min;

  	/* FORMULA FOR CALCULATING HUE*/

  	// Check if the value for delta is a miniscule value - in which case, it can be set to 0
  	if(delta < 0.000001)
    	hue = 0.0;

  	// Else, check if the maximum value is from red index
  	else if(maxInd == 0)
  	{
    	double mod_factor = (gP-bP)/delta;

        while(mod_factor <= 0.0)
            mod_factor = mod_factor + 6.0;

        while(mod_factor >= 6.0)
      		mod_factor = mod_factor - 6.0;

    	hue = mod_factor;
      }

  	// Else, check if the maximum value is from the green index
  	else if(maxInd == 1)
    	hue = (bP - rP)/delta + 2.0;

  	// Else, check if the maximum value is from the blue index
  	else if(maxInd == 2)
    	hue = (rP - gP)/delta + 4.0;

  	// Else, produce an error
  	else
    	cout << "error" << endl;

  	hue = hue * 60.0;

  	/* FORMULA FOR CALCULATING SATURATION */

  	// If the maximum value is a miniscule value, then it can be set to 0
  	if(max < 0.001)
    	sat = 0.0;
    // Else, set the value for saturation
  	else
    	sat = delta/max;

    /* FORMULA FOR CALCULATING VALUE */

 	val = max;   // Set value

  	// Re-establish the values to the address assigned for hue, saturation, and value based on the values calculated through the formulas
  	h = hue;
  	s = sat;
  	v = val;
}

//look up conversion from hsv to rgb for exact formula
void Interface::convertToRGB(double hue, double sat, double val, int &r, int &g, int &b){
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


void Interface::changeHue(int value){
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
void Interface::changeSaturation(int value) {
  if(value > 100 || value < -100){
    cout << "INVALID" << endl;
    return;
  }
  double valdecimal = (double)value / 100.0;
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
      s = s + valdecimal;
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
void Interface::changeBrightness(int value){
  if(value > 100 || value < -100){
  	//WILL MOST LIKELY HAVE TO TAKE OUT
    //cout << "INVALID" << endl;
    return;
  }
  double valdecimal = (double)value / 100.0;
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
      v = v + valdecimal;
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
void Interface::changeContrast(int value){
  if(value < -255 || value > 255){
    //cout << "Error" << endl;
    return;
  }
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


int Interface::clamp(int p){
  if(p < 0)
    return 0;
  else if(p > 255)
    return 255;
  return p;
}


void Interface::changeExposure(int value) {
  if(value > 100 || value < -100){
    //cout << "INVALID" << endl;
    return;
  }
  double valdecimal = (double)value / 100.0;
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
      v = v + valdecimal;
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

void Interface::changeHighlights(int value) {
  if(value > 100 || value < -100){
    cout << "INVALID" << endl;
    return;
  }
  double valdecimal = (double)value / 100.0;
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
      v = v + (valdecimal * v);
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

void Interface::changeShadows(int value) {
  if(value > 100 || value < -100){
    cout << "INVALID" << endl;
    return;
  }
  double valdecimal = (double)value / 100.0;
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
      v = v + (valdecimal * (1-v));
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


void Interface::changeTemperature(int value){
  if(value > 255 || value < -255){
    //cout << "Error" << endl;
    return;
  }
  for (int y = 0; y < height; y++) {
    png_byte* row = row_pointers[y];
    for (int x = 0; x < width; x++) {
      png_byte* ptr = &(row[x*4]);
      ptr[0] = clamp(ptr[0] + value);
      ptr[2] = clamp(ptr[2] - value);
    }
  }
}




void Interface::verticalFlip(){
  png_bytep temp;
  for(int y = 0; y < height/2; y++) {
    temp = row_pointers[y];
    row_pointers[y] = row_pointers[height-1-y];
    row_pointers[height-1-y] = temp;
  }
}

void Interface::rotateLeft(){
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

void Interface::rotateRight(){
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

void Interface::horizontalFlip(){
  rotateLeft();
  rotateLeft();
  verticalFlip();
}
