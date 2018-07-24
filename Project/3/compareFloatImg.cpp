#include <iostream>
#include <cstdint>
#include <string>

#include <fstream>
#include <sstream>

#include <math.h>


using namespace std;

void usage(){
  cerr<<"Usage: compareFloatImg inputImage1 inputImage2 epsilon max_diff_number"<<endl;
}

/** Reads an image for the file inFile. Creates an array of the corect size in the heap.
 *  Returns true on success
 */
bool readImage(float * &image, const char * inFile, unsigned int &width, unsigned int &height){
  ifstream bIn;
  // fix the size of the two dimension variables to 32 bit
  uint32_t width32 = 0;
  uint32_t height32 = 0;
  
  // open the file for binary reading
  bIn.open(inFile, std::ofstream::in | std::ofstream::binary);
  if (!bIn.is_open()){
    cerr<<"Error opening file "<<inFile<<" for reading!"<<endl;
    return false;
  }

  // read the width and height
  bIn.read( (char*)&width32, sizeof(width32));
  bIn.read( (char*)&height32, sizeof(height32));
  // error checking
  if(width32 == 0 || height32 == 0){
    cerr<<"Width or height 0"<<endl;
    return false;
  }

  // setup the image
  width = width32;
  height = height32;
  image = new float[width * height];

  cout<<"Reading width "<<width<<" height  "<<height<<" = "<<width*height<<endl;

  // prepare reading the image
  float * imgInPtr = image;
  float * imgEndPtr = image + width * height;
  // read the input image
  while(bIn && imgInPtr != imgEndPtr){
    // actually read the data. Using a block copy might be faster - but it is quite fast anyways
    bIn.read((char*)imgInPtr++, sizeof(float));
  }
  return true;
}


int main(int argc, char* argv[])
{
  // 1) check the program arguments
  if(argc != 5) {
    usage();
    return 0;
  }

  double epsilon = atof(argv[3]);
  cout<<"Epsilon: "<<epsilon<<endl;

  // image dimensions
  unsigned int width1 = 0;
  unsigned int height1 = 0;
  unsigned int width2 = 0;
  unsigned int height2 = 0;
  
  float * imgIn1 = 0;
  float * imgIn2 = 0;
  
  if(!readImage(imgIn1, argv[1], width1, height1)){
    return 0;
  }
  if(!readImage(imgIn2, argv[2], width2, height2)){
    return 0;
  }
  if(width1 != width2 || height1 != height2){
    cout<<"Image dimensions differ! : "<<width1<<" x "<<height1<<" vs "<<width2<<" x "<<height2<<endl;
    return 1;
  }

  cout<<"Image dimensions: "<<width1<<" x "<<height1<<endl;

  double sum = 0.;

  float *imgPtr1 = imgIn1;
  float *imgPtr2 = imgIn2;
  float *endPtr1 = imgIn1 + (width1 * height1);
  double maxErr = 0.;
  int max_diff_n = atoi(argv[4]);
  cout<<"Maximum different number of points: "<<max_diff_n<<endl;
  int count = 0;
  while(imgPtr1 != endPtr1){
    double diff = fabs( *imgPtr1++ - *imgPtr2++);
    sum += diff;
    if(diff > epsilon && count<max_diff_n){
      cout<<"Difference detected: "<<diff<<endl;
      count+=1;
    }
    if(count>=max_diff_n){
      return 1;
    }
    if(diff > maxErr) maxErr = diff;
  }
  cout<<"Same! Total sum of errors: "<<sum<<"  Maximum error: "<<maxErr<<endl;
  return 0;
}
