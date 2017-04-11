#include <iostream>
#include <png.h>  //libpng
#include <stdio.h>
#include "Picture.h"

using namespace std;

/* I left a massive amount of error handling out of this.  zarb.org/~gc/html/libpng.html*/
void Picture::readPNGFile(){

	/*Opens file and tests that file is a png.*/
	png_byte header[8];  //This will take the first 8 bytes of the file.  Libpng uses the first 8 bytes to determine if a file is a png.
	FILE * file = fopen(fileName, "rb"); //r opens file for reading, b opens it in binary.
	fread(header, 1, 8, file); //fread(pointer to put stuff in, how many bytes per element, how many elements total, file pointer)
  if(!png_sig_cmp(header, 0, 8)){ //png_sig_cmp returns false if file is png, true otherwise.  8 = number of bytes read, I dunno what 0 is but I think it will always be 0.
		isPNG = true;
		cout << "png_sig_cmp worked" << endl;  //aThis is for bug handling, should be deleted before we finish project.
	}
}

/*has errors because I updated variables.*/
// void Picture::writePNGFile(char *outputFile)
// {
// 	FILE *file = fopen(fileName, "wb");
//
// 	pngPointer = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
// 	infoPointer = png_create_info_struct(pngPointer);
//
//
// }

int main(){
	char file[100];  //Will be set to file name.
	cout << "Please enter your file name.  Include the extension." << endl;
	cin >> file;
	Picture picture(file);

	return 0;
}
