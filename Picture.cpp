#include <iostream>
#include <png.h>
#include <stdio.h>

using namespace std;

void Picture::readPNGFile(char *fileName) 
{

}


void Picture::writePNGFile(char *fileName) 
{
	FILE *file = fopen(fileName, "wb");

	pngPointer = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	infoPointer = png_create_info_struct(pngPointer);

	
}