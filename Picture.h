#ifndef Picture_h
#define Picture_h
#include <png.h>

class Picture {
private:
	char * fileName;
	int width, height;
	bool isPNG = false;
	png_byte colorType, bitDepth;  /*colorType is the type of color. (ex. RGB)
	bitDepth is the amount of bits needed to contain information for
	1 pixel of an image.  RGB is normally 8. */

	png_bytep *rowPointer;

public:
	/*libpng works by reading in the file data, applying the actual image data
	to an array, and then printing out that array on a separate file.*/
	Picture(char * inputFile){
		fileName = inputFile;
	};
	void readPNGFile();
	void writePNGFile(char *outputFile);
};

#endif
