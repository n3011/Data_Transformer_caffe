/*!
 * \file iaugmentation_opencv.hpp
 * \modified by Mrinal Haloi
 */
#include <opencv2/opencv.hpp>

namespace DataTransformer {

class ImageAugmenter {
 public:
  // contructor
  ImageAugmenter(void)
      : tmpres(false),
        rotateM(2, 3, CV_32F) {
    rand_crop_ = 0;
    crop_y_start_ = -1;
    crop_x_start_ = -1;
    max_rotate_angle_ = 0.0f;
    max_aspect_ratio_ = 0.0f;
    max_shear_ratio_ = 0.0f;
    min_crop_size_ = -1;
    max_crop_size_ = -1;
    rotate_ = -1.0f;
    max_random_scale_ = 1.0f;
    min_random_scale_ = 1.0f;
    min_img_size_ = 0.0f;
    max_img_size_ = 1e10f;
    fill_value_ = 255;
    multiview_test = 1;
    szFactor = 1;
    sqFactor = 1;
    patchFactor = 1;
  }
  virtual ~ImageAugmenter() {
  }

  cv::Mat getMirrorImg(const cv::Mat& image){
	cv::Mat flipImg;
        cv::flip(image,flipImg,0);
        cv::Point2f imageCenter(flipImg.cols/2.0F, flipImg.rows/2.0F);
        cv::Mat rotMatrix = cv::getRotationMatrix2D(imageCenter, 180.0, 1.0);
        cv::Mat imgMirror(cv::Size(flipImg.size().height, flipImg.size().width), flipImg.type());
        cv::warpAffine(flipImg, imgMirror, rotMatrix, flipImg.size());
//      cv::imshow("flipped",imgMirror);
	
	return imgMirror;	
  }

  cv::Mat cropSquare(const cv::Mat& top_resize1, const int& square_sz, const int& patchFactor){
//	cv::Mat temp1, temp2, temp3, temp4, temp5, temp6;
	switch(patchFactor){
		case 1:
		{
        	cv::Mat temp1 = top_resize1(cv::Rect(0, 0, 224, 224));
			return temp1;
		}
		case 2:
		{
			cv::Mat temp1 = top_resize1(cv::Rect(0, 0, 224, 224));
        	cv::Mat temp_1 = getMirrorImg(temp1); 
			return temp_1;
		}
		case 3:
		{
			cv::Mat temp2 = top_resize1(cv::Rect(0, square_sz - 224, 224, 224));
			return temp2;
		}
		case 4:
		{
			cv::Mat temp2 = top_resize1(cv::Rect(0, square_sz - 224, 224, 224));
        	cv::Mat temp_2 = getMirrorImg(temp2);
			return temp_2;
		}
		case 5:
		{
			cv::Mat temp3 = top_resize1(cv::Rect(square_sz - 224, 0, 224, 224));
			return temp3;
		}
		case 6:
		{
			cv::Mat temp3 = top_resize1(cv::Rect(square_sz - 224, 0, 224, 224));
        	cv::Mat temp_3 = getMirrorImg(temp3);
			return temp_3;
		}
		case 7:
		{
			cv::Mat temp4 = top_resize1(cv::Rect(square_sz - 224, square_sz - 224, 224, 224));
			return temp4;
		}
		case 8:
		{
			cv::Mat temp4 = top_resize1(cv::Rect(square_sz - 224, square_sz - 224, 224, 224));
        	cv::Mat temp_4 = getMirrorImg(temp4);
			return temp_4;
		}
		case 9:
		{
			cv::Mat temp5 = top_resize1(cv::Rect(square_sz/2.0F - 112, square_sz/2.0F - 112, 224, 224));
			return temp5;
		}
		case 10:
		{
			cv::Mat temp5 = top_resize1(cv::Rect(square_sz/2.0F - 112, square_sz/2.0F - 112, 224, 224));
        	cv::Mat temp_5 = getMirrorImg(temp5);
			return temp_5;
		}
		case 11:
		{
			cv::Mat temp6;
        	cv::resize(top_resize1, temp6, cv::Size(224, 224), 0, 0, CV_INTER_CUBIC);
			return temp6;
		}
		case 12:
		{
			cv::Mat temp6;
        	cv::resize(top_resize1, temp6, cv::Size(224, 224), 0, 0, CV_INTER_CUBIC);
        	cv::Mat temp_6 = getMirrorImg(temp6);
			return temp_6;
		}
		default:
		{
			cv::Mat temp5 = top_resize1(cv::Rect(square_sz/2.0F - 112, square_sz/2.0F - 112, 224, 224));
            return temp5;
		}
	}
}

/*
  funcion would return a single partch as per requirment, this function generate patch as per the process described in GoogLeNet paper
  Mutiple resizing option and square selection and corner selection is enabled to this function.
  szFactor: 1-4, to resize the source image to one specified size
  sqFactor: 1-3, to select left, right, center or top, bottom, middle square from resized image
  patchFactor: 1-12, patch for four corner and their mirror or center/resized part and its mirror version
*/

cv::Mat getRandomCrop(const cv::Mat& img, const int& szFactor, const int& sqFactor, const int& patchFactor){

	//create temporary image for resizing
	cv::Mat img_resize1, img_resize2, img_resize3, img_resize4;
	//patch would be returned as per user requirement
	cv::Mat patch;

	//check for protrait/lanscape mode
	if (img.rows > img.cols){
		
		switch(szFactor){
			case 1:
				cv::resize(img, img_resize1, cv::Size(256, img.rows), 0, 0, CV_INTER_CUBIC);
				switch(sqFactor){
					case 1:
					{
						cv::Mat top_resize1 = img_resize1(cv::Rect(0, 0, 256, 256)).clone();
						patch = cropSquare(top_resize1, 256, patchFactor);
						return patch;
					}
					case 2:
					{
						cv::Mat bottom_resize1 = img_resize1(cv::Rect(0, img.rows - 256, 256, 256)).clone();
						patch = cropSquare(bottom_resize1, 256, patchFactor);
						return patch;
					}
					case 3:
					{
						cv::Mat middle_resize1 = img_resize1(cv::Rect(0, img.rows/2.0F - 126, 256, 256)).clone();
						patch = cropSquare(middle_resize1, 256, patchFactor);
						return patch;
					}
				}
			case 2:
				cv::resize(img, img_resize2, cv::Size(288, img.rows), 0, 0, CV_INTER_CUBIC);
				switch(sqFactor){
					case 1:
					{
						cv::Mat top_resize2 = img_resize2(cv::Rect(0, 0, 288, 288)).clone();
						patch = cropSquare(top_resize2, 288, patchFactor);
						return patch;
					}
					case 2:
					{
						cv::Mat bottom_resize2 = img_resize2(cv::Rect(0, img.rows - 288, 288, 288)).clone();
						patch  = cropSquare(bottom_resize2, 288, patchFactor);
						return patch;
					}
					case 3:
					{
						cv::Mat middle_resize2 = img_resize2(cv::Rect(0, img.rows/2.0F - 142, 288, 288)).clone();
						patch  = cropSquare(middle_resize2, 288, patchFactor);
						return patch;
					}
		
				}
			case 3:	
				cv::resize(img, img_resize3, cv::Size(320, img.rows), 0, 0, CV_INTER_CUBIC);
				switch(sqFactor){
					case 1:
					{
						cv::Mat top_resize3 = img_resize3(cv::Rect(0, 0, 320, 320)).clone();
						patch = cropSquare(top_resize3, 320, patchFactor);	
						return patch;
					}
					case 2:
					{
						cv::Mat bottom_resize3 = img_resize3(cv::Rect(0, img.rows - 320, 320, 320)).clone();
						patch = cropSquare(bottom_resize3, 320, patchFactor); 
						return patch;
					}
					case 3:
					{
						cv::Mat middle_resize3 = img_resize3(cv::Rect(0, img.rows/2.0F - 158, 320, 320)).clone();
						patch = cropSquare(middle_resize3, 320, patchFactor);
						return patch;
					}
				}
			case 4:	
				cv::resize(img, img_resize4, cv::Size(352, img.rows), 0, 0, CV_INTER_CUBIC);
				switch(sqFactor){
					case 1:
					{
						cv::Mat top_resize4 = img_resize4(cv::Rect(0, 0, 352, 352)).clone();
						patch = cropSquare(top_resize4, 352, patchFactor);
						return patch;
					}
					case 2:
					{
						cv::Mat bottom_resize4 = img_resize4(cv::Rect(0, img.rows - 352, 352, 352)).clone();
						patch = cropSquare(bottom_resize4, 352, patchFactor);
						return patch;	
					}
					case 3:
					{
						cv::Mat middle_resize4 = img_resize4(cv::Rect(0, img.rows/2.0F - 174, 352, 352)).clone();
						patch = cropSquare(middle_resize4, 352, patchFactor);
						return patch;
					}
				}
				default:
				{
					cv::resize(img, img_resize1, cv::Size(256, img.rows), 0, 0, CV_INTER_CUBIC);
					cv::Mat middle_resize1 = img_resize1(cv::Rect(0, img.rows/2.0F - 126, 256, 256)).clone();
					patch = cropSquare(middle_resize1, 256, patchFactor);
					return patch;
				}
					
			}

	}
	else{

	 switch(szFactor){
                        case 1:
                                cv::resize(img, img_resize1, cv::Size(img.cols, 256), 0, 0, CV_INTER_CUBIC);
                                switch(sqFactor){
                                        case 1:
					{
                                                cv::Mat top_resize1 = img_resize1(cv::Rect(0, 0, 256, 256)).clone();
                                                patch = cropSquare(top_resize1, 256, patchFactor);
                                                return patch;
					}
                                        case 2:
					{
                                                cv::Mat bottom_resize1 = img_resize1(cv::Rect(img.cols - 256, 0, 256, 256)).clone();
                                                patch = cropSquare(bottom_resize1, 256, patchFactor);
                                                return patch;
					}
                                        case 3:
					{
                                                cv::Mat middle_resize1 = img_resize1(cv::Rect(img.cols/2.0F - 126, 0, 256, 256)).clone();
                                                patch = cropSquare(middle_resize1, 256, patchFactor);
                                                return patch;
					}
                                }
                        case 2:
                                cv::resize(img, img_resize2, cv::Size(288, img.rows), 0, 0, CV_INTER_CUBIC);
                                switch(sqFactor){
                                        case 1:
					{
                                                cv::Mat top_resize2 = img_resize2(cv::Rect(0, 0, 288, 288)).clone();
                                                patch = cropSquare(top_resize2, 288, patchFactor);
                                                return patch;
					}
                                        case 2:
					{
                                                cv::Mat bottom_resize2 = img_resize2(cv::Rect(img.cols - 288, 0, 288, 288)).clone();
                                                patch  = cropSquare(bottom_resize2, 288, patchFactor);
                                                return patch;
					}
                                        case 3:
					{
                                                cv::Mat middle_resize2 = img_resize2(cv::Rect(img.cols/2.0F - 142, 0, 288, 288)).clone();
                                                patch  = cropSquare(middle_resize2, 288, patchFactor);
                                                return patch;
					}

                                }
                        case 3:
                                cv::resize(img, img_resize3, cv::Size(320, img.rows), 0, 0, CV_INTER_CUBIC);
                                switch(sqFactor){
                                        case 1:
					{
                                                cv::Mat top_resize3 = img_resize3(cv::Rect(0, 0, 320, 320)).clone();
                                                patch = cropSquare(top_resize3, 320, patchFactor);
                                                return patch;
					}
                                        case 2:
					{
                                                cv::Mat bottom_resize3 = img_resize3(cv::Rect(img.cols - 320, 0, 320, 320)).clone();
                                                patch = cropSquare(bottom_resize3, 320, patchFactor);
                                                return patch;
					}
                                        case 3:
					{
                                                cv::Mat middle_resize3 = img_resize3(cv::Rect(img.cols/2.0F - 158, 0, 320, 320)).clone();
                                                patch = cropSquare(middle_resize3, 320, patchFactor);
                                                return patch;
					}
                                }
                        case 4:
                                cv::resize(img, img_resize4, cv::Size(352, img.rows), 0, 0, CV_INTER_CUBIC);
                                switch(sqFactor){
                                        case 1:
					{
                                                cv::Mat top_resize4 = img_resize4(cv::Rect(0, 0, 352, 352)).clone();
                                                patch = cropSquare(top_resize4, 352, patchFactor);
                                                return patch;
					}
                                        case 2:
					{
                                                cv::Mat bottom_resize4 = img_resize4(cv::Rect(img.rows - 352, 0, 352, 352)).clone();
                                                patch = cropSquare(bottom_resize4, 352, patchFactor);
                                                return patch;
					}
					case 3:
					{
                                                cv::Mat middle_resize4 = img_resize4(cv::Rect(img.cols/2.0F - 174, 0, 352, 352)).clone();
                                                patch = cropSquare(middle_resize4, 352, patchFactor);
                                                return patch;
					}
                                }
					default:
					{
					cv::resize(img, img_resize1, cv::Size(img.cols, 256), 0, 0, CV_INTER_CUBIC);
					cv::Mat middle_resize1 = img_resize1(cv::Rect(img.cols/2.0F - 126, 0, 256, 256)).clone();
					patch = cropSquare(middle_resize1, 256, patchFactor);
					return patch;
				    }
			
			}
		}


}
  int multiview_test;
  /*! for resizing image */
  int szFactor;
  /*! for selecting square from resized image */
  int sqFactor;
  /*! for getting required patch as per GoogLeNet */
  int patchFactor;
};
}  // namespace cxxnet
#endif
