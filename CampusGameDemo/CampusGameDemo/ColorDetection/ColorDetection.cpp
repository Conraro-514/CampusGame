#include"ColorDetection.h"
using namespace std;
using namespace cv;
cv::Mat ColorDetection(cv::Mat img){

    vector<Mat> channels;
    split(img, channels);
    Mat thresholdImage,mask;
    bool enemy_color=0;
    
    //目标颜色判断： true红色 false蓝色    通道相减
    if(enemy_color){
        cv::subtract(channels[2],channels[1],img);
    }else{
        cv::subtract(channels[0],channels[1],img);
    }
    
    //灰度化
    cv::cvtColor(img,img,cv::COLOR_BGR2GRAY);
    //imshow(" ",img);
    
    //2值化
    cv::threshold(img, img, 60, 255, cv::THRESH_BINARY);
    
    // //轮廓增强
	// thresholdImage.copyTo(mask);
	// cv::Canny(mask, mask, 60, 255, 3);
	// cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(3, 3));
	// cv::dilate(mask, mask, element);
	
    return img;
  
}