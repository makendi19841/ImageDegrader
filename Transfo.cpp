//============================================================================
// Name        : Transfo.cpp
// Author      : Amos Makendi
// Version     : 1.0
// Copyrght   : -
// Descriiption : Defined functions
//============================================================================

#include "Transfo.h"
#include  <math.h>

// Performs shuffle
/*
in    : 
return: 
*/

//void shuffle(unsigned int n, std::vector<unsigned int>& v)
//{
//	::srand()
//
//
//
//}

// Performs flipping
/*
in    : input image
return: flipped image
*/

Mat Transfo::flip_image(cv::Mat& img)
{
	Mat flipped(img.size(), img.type());
	for (int i = -1; i < 2; i++) {
		
		flip(img, flipped, i);
	}

	 //flip(img, flipped, 0); Mrror Vertically
	 //flip(img, flipped, +1); Mrror Horitontally
	 //flip(img, flipped, +1); flip vertically and horizontally

	return flipped;
}



// Performs cropping
/*
in    : input image
return: cropped image
*/


Mat Transfo::random_crop( cv::Mat& img, int crop_size ) 
{

	int h_off = 0;
	int w_off = 0;

	const int img_height = img.rows;
	const int img_width  = img.cols;

	/*h_off = (img_height - h) / 2;
	w_off = (img_width  - w) / 2;*/
	h_off = (img_height) / 2;
	w_off = (img_width) / 2;

	cv::Rect roi(w_off, h_off, crop_size, crop_size);
	img = img(roi);

	return img;
}



// Performs rotation 
/*
in     : input image
return : rotated image
*/
Mat Transfo::rotate1( cv::Mat& src, double angle )
{

	// get rotation matrix for rotating the image around its center
	cv::Point2f center(src.cols / 2.0, src.rows / 2.0);
	cv::Mat rot = cv::getRotationMatrix2D(center, angle, 1.0);
	// determine bounding rectangle
	cv::Rect bbox = cv::RotatedRect(center, src.size(), angle).boundingRect();
	// adjust transformation matrix
	rot.at<double>(0, 2) += bbox.width / 2.0 - center.x;
	rot.at<double>(1, 2) += bbox.height / 2.0 - center.y;
	cv::warpAffine(src, src, rot, bbox.size());

	return src;
}
	

// Performs a circular shift in (dx,dy) direction
/*
in       :  input matrix
dx       :  shift in x-direction
dy       :  shift in y-direction
return   :  circular shifted matrix
*/
Mat Transfo::circShift( cv::Mat& in, int dx, int dy )
{
	Mat shifted(in.size(), in.type());
	for (int y = 0; y < in.rows; y++)
		for (int x = 0; x < in.cols; x++) {
			Point2i src(x - dx, y - dy);
			src.x = src.x < in.cols ? src.x : src.x - in.cols;
			src.y = src.y < in.rows ? src.y : src.y - in.rows;
			shifted.at<float>(y, x) = in.at<float>(src);
		}

	return shifted;  
}

// function degrades the given image with gaussian blur and additive gaussian noise
/*
img         :  input image
degradedImg :  degraded output image
filterDev   :  standard deviation of kernel for gaussian blur
snr         :  signal to noise ratio for additive gaussian noise
return      :  the used gaussian kernel
*/
Mat Transfo::degradeImage( cv::Mat& img, Mat& degradedImg, double filterDev, double snr )
{

	int kSize = round(filterDev * 3) * 2 - 1;
	Mat gaussKernel = getGaussianKernel(kSize, filterDev, CV_32FC1);
	//Mat gaussKernel = getGaussianKernel(kSize, filterDev);
	gaussKernel = gaussKernel * gaussKernel.t();

	Mat imgs = img.clone();

	cvtColor(imgs, imgs, CV_BGR2GRAY); /////// ********** ADDED BY MED
	imgs.convertTo(imgs, CV_32FC1);   /////// *********** Added by med

	dft(imgs, imgs, CV_DXT_FORWARD, imgs.rows);

	Mat kernels = Mat::zeros(img.rows, img.cols, CV_32FC1);
	int dx, dy; dx = dy = (kSize - 1) / 2.;
	for (int i = 0; i<kSize; i++) for (int j = 0; j<kSize; j++) kernels.at<float>((i - dy + img.rows) % img.rows, (j - dx + img.cols) % img.cols) = gaussKernel.at<float>(i, j);
	dft(kernels, kernels, CV_DXT_FORWARD);


	mulSpectrums(imgs, kernels, imgs, 0);
	dft(imgs, degradedImg, CV_DXT_INV_SCALE, img.rows);

	Mat mean, stddev;
	meanStdDev(img, mean, stddev);

	Mat noise = Mat::zeros(img.rows, img.cols, CV_32FC1);
	randn(noise, 0, stddev.at<double>(0) / snr);
	degradedImg = degradedImg + noise;
	threshold(degradedImg, degradedImg, 255, 255, CV_THRESH_TRUNC);
	threshold(degradedImg, degradedImg, 0, 0, CV_THRESH_TOZERO);

	return gaussKernel;
}

// Function displays image (after proper normalization)
/*
win   :  Window name
img   :  Image that shall be displayed
cut   :  Cut or scale values outside of [0,255] range
*/
void Transfo::showImage( const char* win, cv::Mat img, bool cut )
{

   Mat tmp = img.clone();

   if (tmp.channels() == 1){
      if (cut){
         threshold(tmp, tmp, 255, 255, CV_THRESH_TRUNC);
         threshold(tmp, tmp, 0, 0, CV_THRESH_TOZERO);
      }else
         normalize(tmp, tmp, 0, 255, CV_MINMAX);
         
      tmp.convertTo(tmp, CV_8UC1);
   }else{
      tmp.convertTo(tmp, CV_8UC3);
   }
   
   imshow(win, tmp);
}

