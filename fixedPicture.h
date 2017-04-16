#ifndef Picture_h
#define Picture_h
#include <png.h>//libpng library - used for file handling of a png file

class Picture {
private:
	int width, height;
	png_byte color_type;
	png_byte bit_depth;
	png_bytep *row_pointers;
	png_structp png_ptr;
	png_infop info_ptr;
	FILE *fp;
	
	int clamp(int p);
	
public:
	/*libpng works by reading in the file data, applying the actual image data
	to an array, and then printing out that array on a separate file.*/
	Picture();
	Picture(char *inputFile);

	void readPNGFile(char *fileName);
	void writePNGFile(char *fileName);
	
	void convertToHSV(int r, int g, int b, double& hue, double& sat, double& val);
	void convertToRGB(double hue, double sat, double val, int& r, int& g, int& b);
	
	void brightness(int value);
	void contrast(int value);
	void exposure(int value);
	void highlights(int value);
	void shadows(int value);
	void saturation(int value);
	void hue(int value);
	void temperature(int value);	
	
	void verticalFlip();
	void rotateLeft();
	void rotateRight();
	void horizontalFlip();
	
};

#endif
