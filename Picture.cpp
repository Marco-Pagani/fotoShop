#include <iostream>
#include <stdio.h> 
#include <png.h> // libpng library - used for file handling of a png file
#include "Picture.h"

using namespace std;

Picture::Picture() {}

/*  Function to open up a PNG picture file and read in its information. This met
 * hod takes in a character array as input to represent the name of the file tha
 * t is to be
opened (the input must be a character array since the libpng libra
 * ry is written in C and cannot handle strings). The function contains no outpu
 * t. It simply opens the PNG
file, and creates a Picture object based on the in
 * formation inside the file. 
 */
void Picture::readPNGFile(char * fileName) {
    
  // Create an array to read in the first 8 bits of a png file. The first 8 bits represents the header portion for a PNG picture.
  png_byte header[8];  //This will take the first 8 bytes of the file.  Libpng uses the first 8 bytes to determine if a file is a png.
  
  //Open a file based on the name passed in
  FILE * x = fopen(fileName, "rb");  //r opens a file for reading, b opens it in binary.
  
  //Read in the header signature of the PNG file
  fread(header, 1, 8, x); //fread(pointer to put stuff in, how many bytes per element, how many elements total, file pointer)
  


  // Check if the file opened is indeed a PNG file
  	/* png_sig_cmp is a library function that checks if a file opened is a PNG picture based on its header.
  	It returns 0 if the first 8 bits of the file indicate that the file is a PNG picture */
  if(!png_sig_cmp(header, 0, 8)) //png_sig_cmp returns false if file is png, true otherwise.  8 = number of bytes read, I dunno what 0 is but I think it will always be 0.
  {
    isPNG = true;  //for verification purposes.
    //ERROR - BREAK FROM CODE
    cout << "png_sig_cmp worked" << endl;  //This is for bug handling.
  }

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
  
  png_set_sig_bytes(png_ptr, 8);  //png_sig_cmp took bytes from the start of the file, this gives them back to the file.
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
    readTrue = false;
  }

  if(readTrue){
    cout << "png_read_image worked" << endl;
  }

  fclose(x);
} 

/* This function opens up a file and writes a PNG file to it. The method takes in a character array as input that will be used to represent the file name (a
character array must be used instead of a string because libpng is written in C and cannot function with strings). The function contains no output.*/
void Picture::writePNGFile(char * fileName) {
    
    // Open up a new file that will be used to write a PNG picture to
    //http://www.libpng.org/pub/png/book/chapter15.html was of especial use.
    FILE * output = fopen(fileName, "wb"); //Creates new file to write to.
    
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
void Picture::convertToHSV(int r, int g, int b, double &h, double &s, double &v)
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
      		mod_factor = mod_factor += 6.0;
    	
    	while(mod_factor >= 6.0){
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

  	return;
}

/* Function for converting the values from hue-saturation-value format to red-green-blue format. The function takes in three double values to represent the values
hue, saturation, and values. The function also takes in three integer address so that the values of red, green, and blue can be changed. The method contains no outputs. */
void Picture::convertToRGB(double hue, double sat, double val, int &r, int &g, int &b)
{
	// Perform calculations to convert the values from hue, saturation, and value to red, green, and blue
  	double c = val * sat;
  	double mod_factor = hue / 60.0;
  	while(mod_factor >= 2.0)
    	mod_factor = mod_factor - 2.0;
  	while(mod_factor <= 0.0)
    	mod_factor = mod_factor + 2.0;

  	double x = c * (1 - abs(mod_factor - 1));

  	double m = val - c;
  
  	// Initalize variables to hold the values for red, green, and blue information
  	double rP, gP, bP;

 	// Based on the value of hue, determine which values should be assigned to red, green, or blue
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
  
  	// Assign the calculated values of red, green, blue back to their memory address
  	r = (int)((rP + m) * 255.0);
  	g = (int)((gP + m) * 255.0);
  	b = (int)((bP + m) * 255.0);

  	return;
}

/* Function to change the hue of a picture. The input of the function is an integer, which represents the how much the hue for the picture should be changed.
The function contains no output. The interface linked with this algorithm is restricted to pass in a value between -359 and 359.*/ 
void Picture::changeHue(int value){

	// If the value passed in is 0, the hue does not need to be altered
  	if(value == 0)
  		return; 

  	// Initalize variables to hold the red-green-blue values, as well as the hue-saturation-value values
  	int r, g, b;
  	double h, s, v;

  	// Establish a loop to parse through each row of the pixels in the picture
  	for (int y = 0; y < height; y++) 
  	{
  		// Store the value for each row of the picture as the loop iterates
    	png_byte* row = row_pointers[y];

    	// Establish a second loop to iterate through each column pixel value
    	for (int x = 0; x < width; x++) 
    	{
    		// Extract the color values of each pixel four values at a time
      		png_byte* ptr = &(row[x*4]);
      		r = ptr[0];   // The red color value is stored within the first index 
      		g = ptr[1];   // The green color value is stored within the second index
      		b = ptr[2];   // The blue color value is stored within the third index

      		convertToHSV(r, g, b, h, s, v);   // Covert the red-green-blue values to hue-saturation-value

      		// Adjust the hue based on the value passed into the function
      		h = h + (double)value;

      		// Check if the new value of hue is within the valid range for hue (The range of hue values exists between 0 and 360)
      		// If the new hue value is above 360, reset it to 359 (the highest possible value for hue)
      		if(h > 360.0)
        		h = 359.0;   
        	// If the new value for hue is negative, rest it to 0
      		if(h < 0)
        		h = 0;

      		convertToRGB(h, s, v, r, g, b);   // Covert the hue-saturation-value information back to red-blue-green based on the new hue value

      		// Reset the value of red, blue, green with the newly calculated hue value for each pixel within the picture
      		ptr[0] = r;
      		ptr[1] = g;
      		ptr[2] = b;
    }
  }
}

//Can only go from 0 to 1, the value is a percentage so must be inbetween -1 and 1.
void Picture::changeSat(int value) {
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
void Picture::changeBright(int value){
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
void Picture::changeContrast(int value){
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


int Picture::clamp(int p){
  if(p < 0)
    return 0;
  else if(p > 255)
    return 255;
  return p;
}


void Picture::changeExposure(int value) {
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

void Picture::changeHighlights(int value) {
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

void Picture::changeShadows(int value) {
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


void Picture::changeTemp(int value){
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
  test.changeTemp(40);
  test.rotateLeft();
  test.changeHue(200);
  test.changeContrast(10);
  //etc.
  
  //Writes to output file
  cout << "Please enter a file name to save the file: ";
  cin >> fileName;
  test.writePNGFile(fileName);
  return 0;
}
