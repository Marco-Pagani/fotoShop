#include <iostream>
#include <png++/png.hpp>
#include <string>

using namespace std;

int main(){
  string fileName;
  string newfileName;
  cout << "Please enter image file name." << endl;
  cin >> fileName;
  png::image< png::rgb_pixel > image(fileName);
  cout << "Please enter new image file name." << endl;
  cin >> newfileName;
  image.write(newfileName);
  cout << "All finished!" << endl;

  return 0;
}
