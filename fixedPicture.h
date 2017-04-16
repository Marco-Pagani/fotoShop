#ifndef Picture_h
#define Picture_h
#include <png.h>

class Picture {
private:
	int width, height;
	png_byte color_type;
	png_byte bit_depth;
	png_bytep * row_pointers;
	png_structp png_ptr;
	png_infop info_ptr;
	
	int clamp(int x);

public:
	/*libpng works by reading in the file data, applying the actual image data
	to an array, and then printing out that array on a separate file.*/
	
	Picture();
	Picture(char *fileName);

	void readPNGFile(char *fileName);
	void writePNGFile(char *fileName);
	
	void convertToHSV(int r, int g, int b, double& hue, double& sat, double& val);
	void convertToRGB(double hue, double sat, double val, int& r, int& g, int& b);
	
	void changeBright(int value);	//done
	void changeContrast(int value);				//done
	void changeExposure(int value);
	void changeHighlights(int value);		//done
	void changeShadows(int value);			//done
	void changeSat(int value);			//done
	void changeHue(int hueVal);				//done
	void changeTemp(int value);			//done
	
	void verticalFlip();
	void rotateLeft();
	void rotateRight();
	void horizontalFlip();
	
};

#endif
