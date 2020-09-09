//============================================================================
// Name        : Main.cpp
// Author      : Amos Makendi
// Version     : 1.0
// Copyright   : -
// Description : main file for image transformation 
//============================================================================


#include <dirent.h>
#include "Transfo.h"

#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <string>

using namespace std;
using namespace cv;

int main(int argc, char**argv) {

	// check if enough arguments

	if (argc < 2) {
		cout << "Not enough arguments!" << endl;
		return -2;
	}

	// construct processing object
	Transfo transfo;

	// define the file name in current directory
	string dirName = "./image2/twa";

	DIR *dir;                                    // pointer to the directory                                   
	dir = opendir(dirName.c_str());

	string imgName;
	struct dirent *ent;

	int counter = 1;

	if (dir != NULL) {
		while ((ent = readdir(dir)) != NULL) {
			imgName = ent->d_name;
			// I found some . and .. files here so I reject them.
			if (imgName.compare(".") != 0 && imgName.compare("..") != 0) {
				string aux;
				aux.append(dirName);
				aux.append("/");
				aux.append(imgName);


				// load images
				cout << "Loading images from current directory....." << endl << endl;
				cout << aux << endl << endl;
				Mat img = imread(aux);

				// display images
				/*namedWindow(" Loaded Image", CV_WINDOW_AUTOSIZE);
				imshow("Loaded Image", img);
				waitKey();*/

				// convert U8 to 32F
				img.convertTo(img, CV_32FC1);


				// degrade image
				double snr = atof(argv[1]);
				double filterDev = atof(argv[2]);

				cout << "Parameters to degrade image..." << endl << endl;
				cout << "Snr: signal-to-noise ratio: the higher (e.g. 10,000), the less noise" << endl;
				cout << "Snr value is:" << snr << endl << endl << endl;

				cout << "Stddev : stddev of Gaussian blur: try value from 0 to saturation" << endl;
				cout << "Stddev is:" << filterDev << endl << endl;


				cout << "Degrading images ongoing..." << endl << endl;
				Mat degradedImg;
				degradedImg.convertTo(degradedImg, CV_32FC1);
				Mat gaussKernel = transfo.degradeImage(img, degradedImg, filterDev, snr);

				// save degraded images 
				bool isSuccess = imwrite("./resultNoise/twa/" + imgName, degradedImg);
				if (isSuccess == false)
				{
					cout << "Failed to save images..." << endl;
					cin.get();
					return -1;
				}

				cout << "Images are successfully saved to the result file..." << endl;
				cout << "Degrading image done" << endl;


				// rotate image
				int rotationAngle = atof(argv[3]);
				cout << "rotationAngle : rotation angle" << endl;
				cout << "rotationAngle is:" << rotationAngle << endl << endl;
				cout << "Rotating images ongoing..." << endl << endl;

				Mat rotateImg;
				rotateImg.convertTo(rotateImg, CV_32FC1);
				rotateImg = transfo.rotate1(img, rotationAngle);


				// Save rotated Image
				bool isSuccess1 = imwrite("./resultRotation/twa/" + imgName, rotateImg);
				if (isSuccess1 == false)
				{
					cout << "Failed to save images..." << endl;
					cin.get();
					return -1;
				}

				cout << "Rotates Images are successfully saved to the result file..." << endl;
				cout << "Rotating image done" << endl;


				// cropping image
				int cropSize = atof(argv[4]);
				cout << "cropSize: crop size" << endl;
				cout << "cropSize is:" << rotationAngle << endl << endl;
				cout << "Cropping images ongoing..." << endl << endl;

				Mat cropImg;
				cropImg.convertTo(cropImg, CV_32FC1);
				cropImg = transfo.random_crop(img, cropSize);

				// Save cropped Image
				bool isSuccess2 = imwrite("./resultCropping/twa/" + imgName, cropImg);
				if (isSuccess2 == false)
				{
					cout << "Failed to save images..." << endl;
					cin.get();
					return -1;
				}

				cout << "Cropped images are successfully saved to the result file..." << endl;
				cout << "Cropping images done" << endl;

				// flipping image
				cout << " Horizontal flipping ongoing..." << endl << endl;

				Mat flipImg;
				flipImg.convertTo(flipImg, CV_32FC1);
				flipImg = transfo.flip_image(img);

				// Save flipped Image
				bool isSuccess3 = imwrite("./resultFlipping/twa/" + imgName, flipImg);
				if (isSuccess3 == false)
				{
					cout << "Failed to save images4..." << endl;
					cin.get();
					return -1;
				}

				cout << "Horizontal flipped images are successfully saved to the result file..." << endl;
				cout << "Horizontal flipping images done" << endl;



			}
		}

		// after all apatial transformation are done
		cout << ".............Spatial Transformation was successful.........." << std::endl << std::endl;

		// close the directory
		closedir(dir);
	}
	else {
		cout << "not present" << endl;
	}


  /*
    **************************  Counting files from folder***************************
  */

	struct stat info;
	std::stringstream pathName;
	pathName.str("");
	pathName.clear();
	pathName << "resultFlipping";

	if (stat(pathName.str().c_str(), &info) == 0){
		// check if resultFlipping folder does exist
		std::string subfolder;
		std::wstring lpcwPath;
		subfolder = pathName.str() + "\\twa\\";

		int counterAllimage = 0;
		std::vector<string>  my_strVector;

		// subfolder exists
		if (stat(pathName.str().c_str(), &info) == 0) {

			// convert path to LPCWSTR(Long Pointer to Constant Wide String) // W: stands for wide and means that the string is stored in a 2bytes 
			try{
				// working with boost-filesystem
				std::string pathForConverting = subfolder + "*";
				int sLength = (int)pathForConverting.length() + 1;
				int len = MultiByteToWideChar(CP_ACP, 0, pathForConverting.c_str(), sLength, 0, 0);
				wchar_t* pathBuffer = new wchar_t[len];
				MultiByteToWideChar(CP_ACP, 0, pathForConverting.c_str(), sLength, pathBuffer, len); // perform the conversion
				lpcwPath = pathBuffer;
				delete[] pathBuffer;
			}
			catch (std::exception& e){
				std::cout << "Error converting image path for folder images: " << e.what() << std::endl;
			}

			// read the file in training image to parse file counter
			WIN32_FIND_DATA findFileData;
			HANDLE hFind = FindFirstFile(lpcwPath.c_str(), &findFileData);

			if (hFind == INVALID_HANDLE_VALUE) {
				// no files found in the folder
				std::cout << "  .\\resultFlipping\\twa\\ is empty" << std::endl;
			}
			else {
				// there are files in the folder
				do{
					std::wstring fileName(findFileData.cFileName);
					std::wstring depthPNG = L"_depth.png";
					

					if (fileName.rfind(depthPNG) != std::string::npos) {
						std::string strFileName = subfolder + std::string(fileName.begin(), fileName.end());

						std::string name = std::string(fileName.begin(), fileName.end());

						my_strVector.push_back(name);

						counterAllimage++;
					}

				} while (FindNextFile(hFind, &findFileData));

				std::cout << " The value of the string vector is: " <<my_strVector[3] << std::endl;
				std::cout << " The size of string vector is: " << my_strVector.size() << std::endl << std::endl;
			}

			
	/*
	**************************Random spliting training images into a percent well defined ***********
	*/

     		// define percentage of the the test_dataset
			int percentTestdata = atof(argv[5]);	
			cout << ".............Random selection images starting.........." << std::endl;
			std::cout << "The number of images is: " << counterAllimage << std::endl;
			cout << "Percentage: " << percentTestdata << std::endl;
		

		   // Ratio of splitting training dataset
			/*int ratio = int((counterAllimage * percentTestdata) / 100);*/
			int ratio = int((my_strVector.size() * percentTestdata) / 100);
			std::cout << "The ratio is : " << ratio << std::endl << std::endl;
			
			
			
			for (int i = 0; i < ratio; i++) 
			{
				//..shuffle (mixed up) my_strVector indexes before selection
				//std::shuffle(my_strVector.begin(), my_strVector.end(), std::default_random_engine{});

				//..shuffle (mixed up) my_strVector indexes before selection
				std::random_device rd;
				std::mt19937 g(rd());
				std::shuffle(my_strVector.begin(), my_strVector.end(), g);

				//...pick up the first image
				std::string result_str = my_strVector[i];

				// display the random choice
				std::cout << " Random selection of image is: " << result_str << std::endl;

				// Go to folder directory to pick the image to save
				string dirName2 = "./resultFlipping/twa";
				DIR *dir2;
				dir2 = opendir(dirName2.c_str());
				string imgName2;
				struct dirent *ent;
				
				// get name of each image in the folder
				if (dir2 != NULL) 
				{
					while ((ent = readdir(dir2)) != NULL)
					{
						imgName2 = ent->d_name;
						if (imgName2.compare(".") != 0 && imgName2.compare("..") != 0)
						{
							string aux2;
							aux2.append(dirName2);
							aux2.append("/");
							aux2.append(imgName2);

							if (aux2.compare(result_str) == 0)
							{
								// load corresponding image
								Mat img2 = imread(aux2);

								std::cout << " I am here...." << std::endl;
								std::cout << " Image width is: " << img2.cols << std::endl;
								std::cout << " Image height is: " << img2.rows << std::endl << std::endl;

								std::cout << " Task done...." << std::endl;

								// display images
								/*namedWindow(" Loaded Image", CV_WINDOW_AUTOSIZE);
								imshow("Loaded Image", img2);
								waitKey();*/


							}

						}

					}
                  
				}
				 

				

            				
			}



		}

		else {
			// folder does not exist
			std::cout << " No existing folder for the random selection...define one" << std::endl;
		}

		char c;
		cin >> c;
	}
}