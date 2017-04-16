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
	
	int clamp(int x);

public:
	/*libpng works by reading in the file data, applying the actual image data
	to an array, and then printing out that array on a separate file.*/
	
	Picture();

	void readPNGFile(char *fileName);
	void writePNGFile(char *fileName);
	
	void convertToHSV(int r, int g, int b, double& hue, double& sat, double& val);
	void convertToRGB(double hue, double sat, double val, int& r, int& g, int& b);
	
	void changeBright(double brightVal);	//done
	void adjustContrast(int c);				//done
	void changeExpo(int ex);
	void changeHighlights(int value);		//done
	void changeShadows(int value);			//done
	void changeSat(double satVal);			//done
	void changeHue(int hueVal);				//done
	void changeWarmth(int value);	
};

#endif
