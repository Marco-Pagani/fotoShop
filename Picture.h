#ifndef Picture_h
#define Picture_h
#include <png.h>

class Picture {
private:
	bool isPNG = false;
	bool readTrue = true;
	int width, height;
	png_byte color_type;
	png_byte bit_depth;
	png_bytep * row_pointers;
	png_structp png_ptr;
	png_infop info_ptr;

public:
	/*libpng works by reading in the file data, applying the actual image data
	to an array, and then printing out that array on a separate file.*/
	Picture(char * inputFile);
	
	void convertToHSL(int& r, int& g, int& b, double h, double s, double l);
	void convertToRGB(double h, double s, double l, int& r, int& g, int& b);
	
	void brightness(int value);
	void contrast(int value);
	void exposure(int value);
	void highlights(int value);
	void shadows(int value);
	void saturation(int value);
	void hue(int value);
	void warmth(int value);
	
	void writePNGFile(char *outputFile);
	
};

#endif
