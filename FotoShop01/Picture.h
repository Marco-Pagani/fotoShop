//#ifndef Picture_h
//#define Picture_h
//#include <png.h>
//#include <string>

//class Picture {
//private:
//	int width, height;
//	png_byte color_type;
//	png_byte bit_depth;
//	png_bytep * row_pointers;
//	png_structp png_ptr;
//	png_infop info_ptr;
//    char * filep;

//	int clamp(int x);

//public:
//	/*libpng works by reading in the file data, applying the actual image data
//	to an array, and then printing out that array on a separate file.*/
	
//	Picture();

//    void set_char(std::string file_name);
//    void readPNGFile();
//    void writePNGFile();
	
//    void convertToHSV(int r, int g, int b, double& hue, double& sat, double& val);
//    void convertToRGB(double hue, double sat, double val, int& r, int& g, int& b);

//    void changeBrightness(int value);
//    void changeContrast(int value);
//    void changeExposure(int value);
//    void changeHighlights(int value);
//    void changeShadows(int value);
//    void changeSaturation(int value);
//    void changeHue(int hueVal);
//    void changeTemperature(int value);

//    void verticalFlip();
//    void rotateLeft();
//    void rotateRight();
//    void horizontalFlip();
//};

//#endif
