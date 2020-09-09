//============================================================================
// Name        : Transfo.h
// Author      : Amos Makendi
// Version     : 1.0
// Copyright   : -
// Description : header files for image transformation 
//============================================================================
#include <fstream>
#include <iostream>
#include <string.h>
#include <dirent.h>
#include <list>

#include<algorithm> // std::shuffle
#include<random>    // std::default_random_clock
#include<iterator> 
#include<filessystem>




#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace std;
using namespace cv;

class Transfo{

   public:
      // constructor
	   Transfo(void){};

      // destructor
      ~Transfo(void){};
          
      // function headers of functions to be implemented
      // --> please edit ONLY these functions here
	  Mat flip_image( cv::Mat& img) ;
	  Mat random_crop( cv::Mat& img, int crop_size );
	  Mat rotate1( cv::Mat& src, double angle );
	  Mat circShift( cv::Mat& in, int dx, int dy );
	  Mat histogramImage( cv::Mat& img );
	  Mat frequencyConvolution( cv::Mat& in, Mat& kernel );
	  void showImage( const char* win, cv::Mat img, bool cut = true );
	  Mat degradeImage( cv::Mat& img, Mat& degradedImg, double filterDev, double snr );
};
