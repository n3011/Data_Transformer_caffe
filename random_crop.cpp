// ntugnet.cpp : Defines the entry point for the console application.
//author: mhaloi@ntu.edu.sg/mrinal.haloi11gmail.com

#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>


using namespace std;
using namespace cv;

Mat getMirrorImg(const Mat& img);
Mat getRandomCrop(const Mat& img, const int& szFactor, const int& sqFactor, const int& patchFactor);
Mat cropSquare(const Mat& top_resize1, const int& square_sz, const int& patchFactor);
Mat getPatch(const Mat& img, const int& szFactor, const int& sqFactor, const int& patchFactor);
void printMatrix(const cv::Mat &M, std::string matrix)
{
	printf("Matrix \"%s\" is %i x %i\n", matrix.c_str(), M.rows, M.cols);
	std::cout << M << std::endl;
}

int main(int argc, char** argv){

	if (argc != 2){
		cout << "please provide image name!" << endl;
		return -1;
	}

	Mat image;
	image = imread(argv[1], CV_LOAD_IMAGE_COLOR);
	Mat ch1;

        vector<Mat> randomCropv;        

	vector<Mat> channels;
	split(image, channels);
	ch1 = channels[0];
	cout<<ch1.size()<<endl;
	cout << ch1.type() <<endl;
	cout << ch1.cols <<endl;
	cout << image.rows <<endl;
	namedWindow("Image", CV_WINDOW_AUTOSIZE);
	imshow("Image", ch1);

	cv::Mat P0(3, 4, CV_32F);
	printMatrix(P0, "P0 Initial");

	vector<Mat> bigm;
	bigm.push_back(P0);

	cv::Mat R0 = cv::Mat::eye(3, 3, CV_32F);
	printMatrix(R0, "R0 I");

	cv::Mat dest(P0.colRange(0, 3));
	R0.copyTo(dest);
	printMatrix(P0, "P0 with R");

	bigm.push_back(R0);
	printMatrix(bigm[1], "ee");

	Mat imgMirror = getMirrorImg(ch1);
	
	Mat patch;
//	patch = getPatch(image, 1,1,1);
        patch = getRandomCrop(image, 1,3,12);
	vector<Mat> ch;
        split(patch,ch);
	cout << patch.size() <<endl;
	namedWindow("re", CV_WINDOW_AUTOSIZE);
	imshow("re", patch);
	
//	cout << randomCropv[11] <<endl;
	waitKey(0);
	return 0;
}

/*
Mat getPatch(Mat img, int szFactor, int sqFactor, int patchFactor){
	vector<Mat> channels, patchChannels;
	Mat patch;
	split(img, channels);
	patchChannels[0] = getRandomCrop(channels[0], szFactor, sqFactor, patchFactor);
	patchChannels[1] = getRandomCrop(channels[1], szFactor, sqFactor, patchFactor);
	patchChannels[2] = getRandomCrop(channels[2], szFactor, sqFactor, patchFactor);
	merge(patchChannels, patch);
	return patch;
}
*/

/*
  funcion would return a single partch as per requirment, this function generate patch as per the process described in GoogLeNet paper
  Mutiple resizing option and square selection and corner selection is enabled to this function.
  szFactor: 1-4, to resize the source image to one specified size
  sqFactor: 1-3, to select left, right, center or top, bottom, middle square from resized image
  patchFactor: 1-12, patch for four corner and their mirror or center/resized part and its mirror version
*/

Mat getRandomCrop(const Mat& img, const int& szFactor, const int& sqFactor, const int& patchFactor){

	//create temporary image for resizing
	Mat img_resize1, img_resize2, img_resize3, img_resize4;
	//patch would be returned as per user requirement
	Mat patch;

	//check for protrait/lanscape mode
	if (img.rows > img.cols){
		
		switch(szFactor){
			case 1:
				resize(img, img_resize1, Size(256, img.rows), 0, 0, INTER_CUBIC);
				switch(sqFactor){
					case 1:
					{
						Mat top_resize1 = img_resize1(Rect(0, 0, 256, 256)).clone();
						patch = cropSquare(top_resize1, 256, patchFactor);
						return patch;
					}
					case 2:
					{
						Mat bottom_resize1 = img_resize1(Rect(0, img.rows - 256, 256, 256)).clone();
						patch = cropSquare(bottom_resize1, 256, patchFactor);
						return patch;
					}
					case 3:
					{
						Mat middle_resize1 = img_resize1(Rect(0, img.rows/2.0F - 127, 256, 256)).clone();
						patch = cropSquare(middle_resize1, 256, patchFactor);
						return patch;
					}
				}
			case 2:
				resize(img, img_resize2, Size(288, img.rows), 0, 0, INTER_CUBIC);
				switch(sqFactor){
					case 1:
					{
						Mat top_resize2 = img_resize2(Rect(0, 0, 288, 288)).clone();
						patch = cropSquare(top_resize2, 288, patchFactor);
						return patch;
					}
					case 2:
					{
						Mat bottom_resize2 = img_resize2(Rect(0, img.rows - 288, 288, 288)).clone();
						patch  = cropSquare(bottom_resize2, 288, patchFactor);
						return patch;
					}
					case 3:
					{
						Mat middle_resize2 = img_resize2(Rect(0, img.rows/2.0F - 143, 288, 288)).clone();
						patch  = cropSquare(middle_resize2, 288, patchFactor);
						return patch;
					}
		
				}
			case 3:	
				resize(img, img_resize3, Size(320, img.rows), 0, 0, INTER_CUBIC);
				switch(sqFactor){
					case 1:
					{
						Mat top_resize3 = img_resize3(Rect(0, 0, 320, 320)).clone();
						patch = cropSquare(top_resize3, 320, patchFactor);	
						return patch;
					}
					case 2:
					{
						Mat bottom_resize3 = img_resize3(Rect(0, img.rows - 320, 320, 320)).clone();
						patch = cropSquare(bottom_resize3, 320, patchFactor); 
						return patch;
					}
					case 3:
					{
						Mat middle_resize3 = img_resize3(Rect(0, img.rows/2.0F - 159, 320, 320)).clone();
						patch = cropSquare(middle_resize3, 320, patchFactor);
						return patch;
					}
				}
			case 4:	
				resize(img, img_resize4, Size(352, img.rows), 0, 0, INTER_CUBIC);
				switch(sqFactor){
					case 1:
					{
						Mat top_resize4 = img_resize4(Rect(0, 0, 352, 352)).clone();
						patch = cropSquare(top_resize4, 352, patchFactor);
						return patch;
					}
					case 2:
					{
						Mat bottom_resize4 = img_resize4(Rect(0, img.rows - 352, 352, 352)).clone();
						patch = cropSquare(bottom_resize4, 352, patchFactor);
						return patch;	
					}
					case 3:
					{
						Mat middle_resize4 = img_resize4(Rect(0, img.rows/2.0F - 175, 352, 352)).clone();
						patch = cropSquare(middle_resize4, 352, patchFactor);
						return patch;
					}
				}
			}

	}
	else{

	 switch(szFactor){
                        case 1:
                                resize(img, img_resize1, Size(img.cols, 256), 0, 0, INTER_CUBIC);
                                switch(sqFactor){
                                        case 1:
					{
                                                Mat top_resize1 = img_resize1(Rect(0, 0, 256, 256)).clone();
                                                patch = cropSquare(top_resize1, 256, patchFactor);
                                                return patch;
					}
                                        case 2:
					{
                                                Mat bottom_resize1 = img_resize1(Rect(img.cols - 256, 0, 256, 256)).clone();
                                                patch = cropSquare(bottom_resize1, 256, patchFactor);
                                                return patch;
					}
                                        case 3:
					{
                                                Mat middle_resize1 = img_resize1(Rect(img.cols/2.0F - 128, 0, 256, 256)).clone();
                                                patch = cropSquare(middle_resize1, 256, patchFactor);
                                                return patch;
					}
                                }
                        case 2:
                                resize(img, img_resize2, Size(288, img.rows), 0, 0, INTER_CUBIC);
                                switch(sqFactor){
                                        case 1:
					{
                                                Mat top_resize2 = img_resize2(Rect(0, 0, 288, 288)).clone();
                                                patch = cropSquare(top_resize2, 288, patchFactor);
                                                return patch;
					}
                                        case 2:
					{
                                                Mat bottom_resize2 = img_resize2(Rect(img.cols - 288, 0, 288, 288)).clone();
                                                patch  = cropSquare(bottom_resize2, 288, patchFactor);
                                                return patch;
					}
                                        case 3:
					{
                                                Mat middle_resize2 = img_resize2(Rect(img.cols/2.0F - 143, 0, 288, 288)).clone();
                                                patch  = cropSquare(middle_resize2, 288, patchFactor);
                                                return patch;
					}

                                }
                        case 3:
                                resize(img, img_resize3, Size(320, img.rows), 0, 0, INTER_CUBIC);
                                switch(sqFactor){
                                        case 1:
					{
                                                Mat top_resize3 = img_resize3(Rect(0, 0, 320, 320)).clone();
                                                patch = cropSquare(top_resize3, 320, patchFactor);
                                                return patch;
					}
                                        case 2:
					{
                                                Mat bottom_resize3 = img_resize3(Rect(img.cols - 320, 0, 320, 320)).clone();
                                                patch = cropSquare(bottom_resize3, 320, patchFactor);
                                                return patch;
					}
                                        case 3:
					{
                                                Mat middle_resize3 = img_resize3(Rect(img.cols/2.0F - 159, 0, 320, 320)).clone();
                                                patch = cropSquare(middle_resize3, 320, patchFactor);
                                                return patch;
					}
                                }
                        case 4:
                                resize(img, img_resize4, Size(352, img.rows), 0, 0, INTER_CUBIC);
                                switch(sqFactor){
                                        case 1:
					{
                                                Mat top_resize4 = img_resize4(Rect(0, 0, 352, 352)).clone();
                                                patch = cropSquare(top_resize4, 352, patchFactor);
                                                return patch;
					}
                                        case 2:
					{
                                                Mat bottom_resize4 = img_resize4(Rect(img.rows - 352, 0, 352, 352)).clone();
                                                patch = cropSquare(bottom_resize4, 352, patchFactor);
                                                return patch;
					}
					case 3:
					{
                                                Mat middle_resize4 = img_resize4(Rect(img.cols/2.0F - 175, 0, 352, 352)).clone();
                                                patch = cropSquare(middle_resize4, 352, patchFactor);
                                                return patch;
					}
                                }

			}
		}


}


Mat cropSquare(const Mat& top_resize1, const int& square_sz, const int& patchFactor){
//	Mat temp1, temp2, temp3, temp4, temp5, temp6;
	switch(patchFactor){
		case 1:
		{
        		Mat temp1 = top_resize1(Rect(0, 0, 224, 224));
			return temp1;
		}
		case 2:
		{
			Mat temp1 = top_resize1(Rect(0, 0, 224, 224));
        		Mat temp_1 = getMirrorImg(temp1); 
			return temp_1;
		}
		case 3:
		{
			Mat temp2 = top_resize1(Rect(0, square_sz - 224, 224, 224));
			return temp2;
		}
		case 4:
		{
			Mat temp2 = top_resize1(Rect(0, square_sz - 224, 224, 224));
        		Mat temp_2 = getMirrorImg(temp2);
			return temp_2;
		}
		case 5:
		{
			Mat temp3 = top_resize1(Rect(square_sz - 224, 0, 224, 224));
			return temp3;
		}
		case 6:
		{
			Mat temp3 = top_resize1(Rect(square_sz - 224, 0, 224, 224));
        		Mat temp_3 = getMirrorImg(temp3);
			return temp_3;
		}
		case 7:
		{
			Mat temp4 = top_resize1(Rect(square_sz - 224, square_sz - 224, 224, 224));
			return temp4;
		}
		case 8:
		{
			Mat temp4 = top_resize1(Rect(square_sz - 224, square_sz - 224, 224, 224));
        		Mat temp_4 = getMirrorImg(temp4);
			return temp_4;
		}
		case 9:
		{
			Mat temp5 = top_resize1(Rect(square_sz/2.0F - 112, square_sz/2.0F - 112, 224, 224));
			return temp5;
		}
		case 10:
		{
			Mat temp5 = top_resize1(Rect(square_sz/2.0F - 112, square_sz/2.0F - 112, 224, 224));
        		Mat temp_5 = getMirrorImg(temp5);
			return temp_5;
		}
		case 11:
		{
			Mat temp6;
        		resize(top_resize1, temp6, Size(224, 224), 0, 0, INTER_CUBIC);
			return temp6;
		}
		case 12:
		{
			Mat temp6;
        		resize(top_resize1, temp6, Size(224, 224), 0, 0, INTER_CUBIC);
        		Mat temp_6 = getMirrorImg(temp6);
			return temp_6;
		}
		default:
		{
			Mat temp5 = top_resize1(Rect(square_sz/2.0F - 112, square_sz/2.0F - 112, 224, 224));
                        return temp5;
		}
	}
}

Mat getMirrorImg(const Mat& image){
	Mat flipImg;
        flip(image,flipImg,0);
        Point2f imageCenter(flipImg.cols/2.0F, flipImg.rows/2.0F);
        Mat rotMatrix = getRotationMatrix2D(imageCenter, 180.0, 1.0);
        Mat imgMirror(Size(flipImg.size().height, flipImg.size().width), flipImg.type());
        warpAffine(flipImg, imgMirror, rotMatrix, flipImg.size());
//      imshow("flipped",imgMirror);
	
	return imgMirror;	
}

