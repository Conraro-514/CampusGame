#include"ColorDetection.h"
using namespace std;
using namespace cv;
cv::Mat ColorDetection(cv::Mat img){

    vector<Mat> channels;
    split(img, channels);
    Mat thresholdImage,mask;
    bool enemy_color=1;
    
    //目标颜色判断： true红色 false蓝色    通道相减
    if(enemy_color){
        cv::subtract(channels[2],channels[0],img);
    }else{
        cv::subtract(channels[0],channels[2],img);
    }
    
    
    //2值化
    cv::threshold(img, img, 60, 255, cv::THRESH_BINARY);
    
    //轮廓增强
	
	// cv::Canny(mask, mask, 60, 255, 3);
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(4, 4));
	// cv::erode(img, img, element);
    cv::morphologyEx(img, img, cv::MORPH_OPEN, element);
    cv::dilate(img, img, element);
	
    return img;
  
}