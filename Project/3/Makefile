.PHONY: all clean

all:  KNNFloatImg compareFloatImg floatImage2png
	
incs=-I /home/include/png++/0.2.5_1/include 

floatImage2png: floatImage2png.cpp
	g++ -fopenmp -O3 -g -std=c++11 $(incs) -o floatImage2png floatImage2png.cpp `libpng-config --cflags` `libpng-config --ldflags`

KNNFloatImg: KNNFloatImg.cpp
	g++ -fopenmp -O3 -g -std=c++11 $(incs) -o KNNFloatImg KNNFloatImg.cpp

compareFloatImg: compareFloatImg.cpp
	g++ -fopenmp -O3 -g -std=c++11 $(incs) -o compareFloatImg compareFloatImg.cpp
	
clean:	
	rm -rf floatImage2png floatImage2png *.dSYM
