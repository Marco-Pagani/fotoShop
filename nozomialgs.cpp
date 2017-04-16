/*These were altered by Nozomi from the github algorithms.  Drop them in the Picture class if they work correctly.*/

//convert rgb to hsl
void convertToHSV(int r, int g, int b, double &h, double &s, double &v){
  double rP = (double)r/255.0;
  double gP = (double)g/255.0;
  double bP = (double)b/255.0;
  double max = -1.0;//smallest number to start with
  double min = 400.0;//outrageously large number initially
  double hue = 0.0;
  double sat = 0.0;
  double val = 0.0;

  double RGB [3] = {rP, gP, bP};

  //convert to 8-bit color depth
  int maxInd;
  for (int i=0;i< 3;i++){
    if (RGB[i] < min)
      min = RGB[i];

    if (RGB[i] > max){
      max = RGB[i];
      maxInd = i;
    }
  }

  double delta = max - min;

  if(delta < 0.000001){
    hue = 0.0;
  }
  else if(maxInd == 0){
    double mod_factor = (gP-bP)/delta;
    while(mod_factor <= 0.0){
      mod_factor = mod_factor += 6.0;
    }
    while(mod_factor >= 6.0){
      mod_factor = mod_factor - 6.0;
    }

    hue = mod_factor;
  }

  else if(maxInd == 1){
    hue = (bP - rP)/delta + 2.0;
  }
  else if(maxInd == 2){
    hue = (rP - gP)/delta + 4.0;
  }
  else{
    cout << "error" << endl;
  }

  hue = hue * 60.0;

  if(max < 0.001){
    sat = 0.0;
  }

  else{
    sat = delta/max;
  }

  val = max;

  h = hue;
  s = sat;
  v = val;

}


//this method converts from HSV to RGB
void convertToRGB(double hue, double sat, double val, int &r, int &g, int &b){
  double c = val * sat;
  double mod_factor = hue / 60.0;
  while(mod_factor >= 2.0)
    mod_factor = mod_factor - 2.0;
  double x = c * (1 - abs(mod_factor - 1));
  double m = val - c;

  double rP, gP, bP;
  if((0.0 <= hue && hue < 60.0)){
      rP = c;
      gP = x;
      bP = 0;
  }
  else if(60.0 <= hue && hue < 120.0){
      rP = x;
      gP = c;
      bP = 0;
  }
  else if(120.0 <= hue && hue < 180.0){
      rP = 0;
      gP = c;
      bP = x;
  }
  else if(180.0 <= hue && hue < 240.0){
      rP = 0;
      gP = x;
      bP = c;
  }
  else if(240.0 <= hue && hue < 300.0){
      rP = x;
      gP = 0;
      bP = c;
  }
  else if(300.0 <= hue && hue < 360.0){
      rP = c;
      gP = 0;
      bP = x;
  }

  r = (int)floor(((rP + m) * 255.0));
  g = (int)floor(((gP + m) * 255.0));
  b = (int)floor(((bP + m) * 255.0));

  if(r > 255 || g > 255 || b > 255){
    r = r % 255;
    g = g % 255;
    b = b % 255;
  }
}


//HUEVAL CAN GO FROM 0 -> 360.
void changeHue(int hueVal) {
  //restricts input to a closed interval (slider)
  if(hueVal > 360 || hueVal < -359){
    cout << "INVALID" << endl;
    return;
  }
  int r, g, b;
  double h, s, v;
  for (int y = 0; y < height; y++) {
    png_byte* row = row_pointers[y];
    for (int x = 0; x < width; x++) {
      png_byte* ptr = &(row[x*4]);
      r = ptr[0];
      g = ptr[1];
      b = ptr[2];
      convertToHSV(r, g, b, h, s, v);
      h = h + (double)hueVal;
      //fixes the max and min values of the possible hue. anything that may go over is stopped exactly at the max.
      if(h > 360.0)
        h = 359.0;
      if(h < 0)
        h = 0;
      convertToRGB(h, s, v, r, g, b);
      ptr[0] = r;
      ptr[1] = g;
      ptr[2] = b;
    }
  }
}

//Can only go from 0 to 1, the value is a percentage so must be inbetween -1 and 1.
void changeSat(double satVal) {
  if(satVal > 1 || satVal < -1){
    cout << "INVALID" << endl;
    return;
  }
  int r, g, b;
  double h, s, v;
  for (int y = 0; y < height; y++) {
    png_byte* row = row_pointers[y];
    for (int x = 0; x < width; x++) {
      png_byte* ptr = &(row[x*4]);
      r = ptr[0];
      g = ptr[1];
      b = ptr[2];
      convertToHSV(r, g, b, h, s, v);
      s = s + satVal;
      //anything set over the possible max is made to be exactly at the min/max, which in this case is 0 and 1.
      if(s > 1.0)
        s = 1.0;
      if(s < 0)
        s = 0;
      convertToRGB(h, s, v, r, g, b);
      ptr[0] = r;
      ptr[1] = g;
      ptr[2] = b;
    }
  }
}

//Adjusts brightness of image.Likewise to saturation, value can only go from 0 to 1 so input must be restricted from -1 to 1.
void changeBright(double brightVal) {
  if(brightVal > 1 || brightVal < -1){
    cout << "INVALID" << endl;
    return;
  }
  int r, g, b;
  double h, s, v;
  for (int y = 0; y < height; y++) {
    png_byte* row = row_pointers[y];
    for (int x = 0; x < width; x++) {
      png_byte* ptr = &(row[x*4]);
      r = ptr[0];
      g = ptr[1];
      b = ptr[2];
      convertToHSV(r, g, b, h, s, v);
      v = v + brightVal;
      //anything set over the possible max is made to be exactly at the max instead of over/under the max.
      if(v > 1.0)
        v = 1.0;
      if(v <= 0.0)
        v = 0.0;
      convertToRGB(h, s, v, r, g, b);
      ptr[0] = r;
      ptr[1] = g;
      ptr[2] = b;
    }
  }
}

//this method checks if the color values are between 0 and 255
int clamp(int x)
{
  if (x < 0)
  {
    return 0;
  }
  else if (x > 255)
  {
    return 255;
  }
  else
  {
    return x;
  }
}


//this method will adjust the contrast
//int c is to be between the values of -255 to 255. "-" reduces contrast,
// "+" increases it

//This is what I found on github about a contrast method. It had a couple errors so I think I fixed them (hopefully without messing up the logic
//However I don't think that c should be allowed to go up to 255, since it messes up the image pretty badly after around 50 (as you might see if you try it yourself)
//Don't even get me started on what happens when you set c to 200 (my eyes!)
void adjustContrast(int c){
  double cFactor = (259.0*((double)c + 255.0)) / (255.0*(259.0 - (double)c));
  int r, g, b;
  for (int y = 0; y < height; y++) {
    png_byte* row = row_pointers[y];
    for (int x = 0; x < width; x++) {
      png_byte* ptr = &(row[x*4]);
      r = ptr[0];
      g = ptr[1];
      b = ptr[2];

      r = (int)(cFactor*(r - 128.0) + 128);
      g = (int)(cFactor*(g - 128.0) + 128);
      b = (int)(cFactor*(b - 128.0) + 128);

      ptr[0] = r;
      ptr[1] = g;
      ptr[2] = b;

    }
  }
}
