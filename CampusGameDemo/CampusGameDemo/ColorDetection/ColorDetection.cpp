#include"ColorDetection.h"

cv::Mat ColorDetection(cv::Mat img,bool enemy_color){

    std::vector<cv::Mat> channels;
    cv::split(img, channels);
    cv::Mat thresholdImage,mask;
    
    //灰度化
    cv::CvtColor(img,img,cv::COLOR_BGR2GRAY);
    
    //2值化
    cv::adaptiveThreshold(img, img, 127, cv::ADAPTIVE_THRESH_GAUSSIAN_C, cv::THRESH_BINARY, 3, 10);
    
    //目标颜色判断： true红色 false蓝色    通道相减
    if(enemy_color()){
        thresholdImage=channels.at(2)-channels.at(1);
    }
    else if(!enemy_color()){
        thresholdImage=channels.at(0)-channels.at(2);
    }
    
    //轮廓增强
	thresholdImage.copyTo(mask);
	cv::Canny(mask, mask, 60, 255, 3);
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
	cv::dilate(mask, mask, element);
	
    return mask;
  
}