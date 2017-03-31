#include <iostream>
#include <png.h>  // Libpng
#include <fstream>
#include <string>
#include <stdio.h>  //Used for FILE, fopen
using namespace std;

//COPIED CODE WE NEED TO WRITE OUR OWN (zarb.org/~gc/html/libpng.html)
//use a file found in the same folder as the .out for this.  It makes things easier.
bool isPNG = false;

void read_png_file(char * file_name){  //must take char * because libpng is written in c,not c++.
  png_byte header[8];  //This will take the first 8 bytes of the file.  Libpng uses the first 8 bytes to determine if a file is a png.
  FILE * x = fopen(file_name, "rb");  //r opens a file for reading, b opens it in binary.
  fread(header, 1, 8, x); //fread(pointer to put stuff in, how many bytes per element, how many elements total, file pointer)
  if(!png_sig_cmp(header, 0, 8)){ //png_sig_cmp returns false if file is png, true otherwise.  8 = number of bytes read, I dunno what 0 is but I think it will always be 0.
    isPNG = true;  //for verification purposes.
  }

}

int main(){
  char file[100];  //file name is put in here.  Must be char * because libpng is in c.
  cout << "Gimme dat file name" << endl;
  cin >> file;
  read_png_file(file);
  if(isPNG){
    cout << "It worked" << endl;
  }
  return 0;
}
