

// these methods will pe part of the picture class


//this method converts from RGB to HSL
//return an int array instead, please fix
void convertToHSL(int& r, int& g, int& b, double h, double s, double l)
{
  double rP = r / 255.0;
  double gP = g / 255.0;
  double bP = b / 255.0;
  double max = 0.0;
  double min = 0.0;
  double hue = 0.0;
  double sat = 0.0;
  double lum = 0.0;
  
  double RGB [3] = {rP, gP, bP};
  double HSL [3] = {hue, sat, lum};
  
  //convert to 8-bit color depth
  for (int i=0;i<RGB.length();i++)
  {
    if (RGB[i] < min)
    {
      min = RGB[i];
    }
    if (RGB[i] > max)
    {
      max = RGB[i];
    }
  }
  
  //calculate Luminance
  lum = (max + min)/2.0;
  
  //calculate Saturation
  if(min == max)
  {
    sat = 0.0;
  }
  else if (lum < 0.5)
  {
    sat = (max - min) / (max + min);
  }
  else if (lum > 0.5)
  {
    sat = (max-min) / (2.0 - max - min);
  }
  else
  {
    cout << "Something went wrong with conversion" << endl;
  }
  
  
  //calculate Hue
  if (sat == 0)
  {
    hue = 0.0;
  }
  else if (rP == max)
  {
    hue = (gP - bP) / (max - min);
  }
  else if (gP == max)
  {
    hue = (2.0 + (bP - rP)) / (max - min);
  }
  else if (bP == max)
  {
    hue = (4.0 + (rP + gP)) / (max - min);
  }
  else
  {
    cout << "Something went wrong with conversion" << endl;
  }
  
  hue = hue * 60;
  
  if (hue < 0)
  {
    hue = hue + 360;
  }
  
}


//this method converts from HSL to RGB

void convertToRGB(double h, double s, double l, int& r, int& g, int& b)
{
  double hue = x;
  double sat = y;
  double lum = z;
  double r = 0.0;
  double g = 0.0;
  double b = 0.0;
  double temp_1 = 0.0;
  double temp_2 = 0.0;
  double temp_r = 0.0;
  double temp_g = 0.0;
  double temp_b = 0.0;
  double HSL [3] = {hue, sat, lum};
  double RGB [3] = {r, g ,b};
  double temp [3] = {temp_r, temp_g, temp_b};
  
  
  if (sat == 0)
  {
    for(int i=0;i<RGB.length();i++)
    {
      RBG[i] = lum * 255;
    }
  }
  
  if (lum < .5)
  {
    temp_1 = lum * (1 + sat);
  }
  else if (lum >= .5)
  {
    temp_1 = lum + sat - (lum * sat);
  }
  
  temp_2 = (2.0 * lum) - temp_1;
  
  hue = hue / 360;
  
  temp_r = hue + 0.333;
  temp_g = hue;
  temp_b = hue - 0.333;
  
  for(int i=0;i<temp.length();i++)
  {
    if(temp[i] < 0)
    {
      temp[i]++;
    }
    else if (temp[i] > 1)
    {
      temp[i] = temp[i] - 1;
    }
  }
  
  for(int i=0;i<RGB.length();i++)
  {
    if((6 * temp[i]) < 1)
    {
      RGB[i] = temp_2 + (temp_1 - temp_2) * 6 * temp[i];
    }
    else if((2 * temp[i]) < 1)
    {
      RGB[i] = temp_1;
    }
    else if((3 * temp[i]) < 2)
    {
      RGB[i] = temp_2 + (temp_1 - temp_2) * (0.666 - temp[i]) * 6;
    }
    
    RGB[i] = RGB[i] * 255;
  }





}

//Matthew Sullivan
//These functions are not the final functions. Still waiting on what the arguments need to be
//As of now, these take in the image along with a value the user wants to change either the
//hue, saturation, or luminosity and changes these by the value given by the user
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

int x, y;
int width, height;
int r, g, b;

void changeHue(png_byte* image, int hueVal) {
  for (y = 0; y < height; y++) {
    png_byte* row = row_pointers[y];
    for (x = 0; x < width; x++) {
      png_byte* ptr = &(row[x*4]);
      r = ptr[0];
      g = ptr[1];
      b = ptr[2];
      double HSL[3] = convertToHSL(r, g, b);
      HSL[0] = HSL[0] + hueVal;
      double RGB[3] = convertToRGB(HSL[0], HSL[1], HSL[2]);
      ptr[0] = RGB[0];
      ptr[1] = RGB[1];
      ptr[2] = RGB[2];
    }
  }
}

void changeSat(png_byte* image, int satVal) {
  for (y = 0; y < height; y++) {
    png_byte* row = row_pointers[y];
    for (x = 0; x < width; x++) {
      png_byte* ptr = &(row[x*4]);
      r = ptr[0];
      g = ptr[1];
      b = ptr[2];
      double HSL[3] = convertToHSL(r, g, b);
      HSL[1] = HSL[1] + satVal;
      double RGB[3] = convertToRGB(HSL[0], HSL[1], HSL[2]);
      ptr[0] = RGB[0];
      ptr[1] = RGB[1];
      ptr[2] = RGB[2];
    }
  }
}

void changeLum(png_byte* image, int lumVal) {
  for (y = 0; y < height; y++) {
    png_byte* row = row_pointers[y];
    for (x = 0; x < width; x++) {
      png_byte* ptr = &(row[x*4]);
      r = ptr[0];
      g = ptr[1];
      b = ptr[2];
      double HSL[3] = convertToHSL(r, g, b);
      HSL[2] = HSL[2] + lumVal;
      double RGB[3] = convertToRGB(HSL[0], HSL[1], HSL[2]);
      ptr[0] = RGB[0];
      ptr[1] = RGB[1];
      ptr[2] = RGB[2];
    }
  }
}




