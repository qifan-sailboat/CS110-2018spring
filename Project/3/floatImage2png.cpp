#include <iostream>
#include <cstdint>
#include <string>

#include <fstream>
#include <sstream>

#include <png++/png.hpp>

#include <math.h>


using namespace std;

void usage(){
  cerr<<"Usage: floatImage2png inputImage outputImage maxRange"<<endl;
}


ifstream bIn;
char mode=' ';  

int main(int argc, char* argv[])
{
  if(argc != 4) {
    usage();
    return 0;
  }

  uint32_t width = 0;
  uint32_t height = 0;
  
  bIn.open(argv[1], std::ofstream::in | std::ofstream::binary);
  if (!bIn.is_open()){
    cerr<<"Error opening file "<<argv[2]<<" for reading!"<<endl;
    return 0;
  }
  
  float maxRange = 0.f;
  maxRange = atof(argv[3]);

  bIn.read( (char*)&width, sizeof(width));
  bIn.read( (char*)&height, sizeof(height));
  if(width == 0 || height == 0){
    cerr<<"Width or height 0"<<endl;
    return 1;
  }
  cout<<"Reading width "<<width<<" x "<<height<<" = "<<width*height<<endl;

  float tmp;
  float max = 0;
  while(bIn){
    bIn.read((char*)&tmp, sizeof(tmp));
    if(tmp > max) max = tmp;
  }
  cout<<"maximum float found: "<<max<<endl;

  if (max > maxRange){
    max = maxRange;
    cout<<"maximum range capped at "<<maxRange<<endl;
  }

  bIn.close();

  bIn.open(argv[1], std::ofstream::in | std::ofstream::binary);
  if (!bIn.is_open()){
    cerr<<"Error opening file "<<argv[2]<<" for reading!"<<endl;
    return 0;
  }
  png::image< png::rgb_pixel > rgb(width, height);

  max = 1. / max;
  // bIn.seekg (0, bIn.beg);
  int val;
  uint col = 0;
  uint row = 0;
  while(bIn){
    if(col == height) break;
    bIn.read((char*)&tmp, sizeof(tmp));
    val = tmp * max * 255.f;
    if(val > 255) val = 255;
    rgb[col][row] = png::rgb_pixel(val, val, val);
    row++;
    if(row == width){
      row = 0;
      col++;
    }
  }

  rgb.write(argv[2]);
 
  return 0;
}
