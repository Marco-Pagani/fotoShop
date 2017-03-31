class Picture {
private: 
	int width, height;
	png_byte colorType, bitDepth;
	
	png_byte *rowPointer;

public:
	void readPNGFile(char *fileName);
	void writePNGFile();
};