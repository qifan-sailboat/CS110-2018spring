#include <iostream>
#include <cstdlib>
#include <cstdint>
#include <string>
#include <algorithm>
#include <fstream>
#include <sstream>

#include <math.h>
#include <chrono>
#include <omp.h>
#include <cfloat>

using namespace std;

void usage(){
	cerr<<"Usage: KNNFloatImg inputImage outputImage k  distance_thr kernelSize(should be 511 but you can use 21 to debug)"<<endl;
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


/* writes the float array img to outFile (as binary floats, first 32bit width and height unsigned integers)
*/
bool writeImage(const float * img, const char * outFile, const unsigned int width, const unsigned int height){
	ofstream bOut;
	// open the file for writing
	bOut.open(outFile,  std::ofstream::binary);
	if (!bOut.is_open()){
		cerr<<"Error opening file "<<outFile<<" for writing!"<<endl;
		return false;
	}

	// write the width and height
	uint32_t width32 = width;
	uint32_t height32 = height;
	bOut.write((char*)&width32, sizeof(width32));
	bOut.write((char*)&height32, sizeof(height32));

	// write the data
	const float * imgPtr = img;
	const float * endPtr = img + (width*height);
	while(imgPtr != endPtr){
		bOut.write((const char*)imgPtr++, sizeof(float));
	}
	return true;
}

static int counter = 0;

/** Given the input image, a coordinate therein, the width and height of the image, the kernel and the size of the kernel (one dimension), the knn number k,
 * this function returns:
 *   the origin image OR
 *   0.f (invalid) if knn regard it as an outlier.
 */
float KNN_removalValue(const float * imgIn, const unsigned int xIn, const unsigned int yIn, float * kernel, const unsigned int width,  const unsigned int height, const unsigned int kernelSize, const unsigned int k, const float distance_thr){
	int halfSize = kernelSize / 2;
	float missingKernelVals = 0.f;
	float k_sum = 0.f;
	float mean  = 0.f;

	const float center_depth = imgIn[xIn + yIn *width];
	const float angle_width_per_pixel = M_PI * 2. / width ;
	const float angle_height_per_pixel = M_PI * 2. / height ;

	const float depth_squared = center_depth * center_depth;
	// Law of cosines ....
	/*
	cout << "angle_width_per_pixel = " << angle_width_per_pixel
		<< " angle_height_per_pixel = " << angle_height_per_pixel << endl;
	*/
	const float width_per_pixel = sqrt( 2*depth_squared - (cos(angle_width_per_pixel)*2*depth_squared) );
	const float height_per_pixel = sqrt( 2*depth_squared - (cos(angle_height_per_pixel)*2*depth_squared) );

	// loop through the kernel
	//#pragma omp parallel for
	for(int x = 0; x < kernelSize; ++x){
		//#pragma omp parallel for
		for(int y = 0; y < kernelSize; ++y){
			// get the value from the image
			kernel[ x + y * kernelSize] = fabs(imgIn[ xIn - halfSize + x  + (yIn - halfSize + y) * width] - center_depth)
			+ width_per_pixel*abs(x - halfSize) + height_per_pixel*abs(y - halfSize);
		}
	}

	for (int i = 0; i < kernelSize * kernelSize; i++){
		if (kernel[i] == 0.f) kernel[i] = FLT_MAX;
	}

	std::nth_element(kernel, kernel + k, kernel + kernelSize * kernelSize);

	for (int i = 0; i < k; i++){
		k_sum += kernel[i];
	}
	
	mean = k_sum / k;
	if ( mean > distance_thr ){
		counter++;
		return 0.f;
	}
	return imgIn[xIn + yIn *width];
}

/** KNN_removal the depth pixels in a depth image
 *
 */
void KNN_removal(const float * imgIn, float * imgOut, const unsigned int &width, const unsigned int &height,  float *kernel, const unsigned int &kernelSize, const unsigned int k, const float distance_thr){

	int halfSize = kernelSize / 2;
	int x, y, count = 0;
	// go through all pixels
	#pragma omp parallel for private(y)
	for(x = 0; x < width; ++x){
		float *temp = new float[kernelSize*kernelSize];
		for(y = 0; y < height; ++y){
		// because of the size of the kernel we can not work on the edge of the depth image
			if(x >= halfSize && y >= halfSize && x < width-halfSize && y < height-halfSize){
			// check if this pixel is in fact unknown == 0
				if(imgIn[x + y*width] != 0.f){
					// this is an known pixel - KNN_removal it
					imgOut[x + y*width] = KNN_removalValue(imgIn, x, y, temp, width, height, kernelSize, k, distance_thr);
					//imgOut[x + y*width] = KNN_removalValue(imgIn, x, y, kernel, width, height, kernelSize, k, distance_thr);
				}else{
					// this pixel is invalid - just copy it
					imgOut[x + y*width] = imgIn[x + y*width];
				}
			}
		}
	}
}


/** The main function. Five steps:
 *  1) check the program arguments
 *  2) create the Kernel
 *  3) read the input image
 *  4) KNN_removal
 *  5) write the output image
 *
 */
int main(int argc, char* argv[])
{
	// 1) check the program arguments
	if(argc != 6) {
		usage();
		return 0;
	}

	unsigned int kernelSize = 511;
	// this 511 is what the kernel should really be.
	kernelSize = atoi(argv[5]);
	// image dimensions
	unsigned int width = 0;
	unsigned int height = 0;
	unsigned int k = 0;
	float distance_thr = 0;
	k = atoi(argv[3]);
	distance_thr = atof(argv[4]);
	// 2) create the kernel
	if(kernelSize%2 == 0){
		cout<<"Kernel size must be odd!"<<endl;
		return 0;
	}
	float * kernel = new float[kernelSize*kernelSize];


	float * imgIn = 0;


	// 3) read the image
	if(!readImage(imgIn, argv[1], width, height)){
		return 0;
	}

	float * imgOut = new float[width * height];

	std::chrono::high_resolution_clock::time_point start_time = chrono::high_resolution_clock::now();

	// 4) KNN_removal the image
	KNN_removal(imgIn, imgOut, width, height, kernel, kernelSize, k, distance_thr);

	std::chrono::high_resolution_clock::time_point end_time = chrono::high_resolution_clock::now();

	unsigned int microsecs = chrono::duration_cast<chrono::microseconds>(end_time - start_time).count();

	cout<<" Micro seconds: "<<microsecs<<endl;

	cout<<"Outliers: "<<counter<<endl;

	// 5) write the output
	writeImage(imgOut, argv[2], width, height);

	return 0;
}
