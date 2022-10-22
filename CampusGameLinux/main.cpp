#include <stdio.h>  
#include <time.h>  
#include <opencv2/opencv.hpp>  
#include <iostream> 
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/ml/ml.hpp>  
//#include <io.h>

#include"GetContours/GetContours.h"
//预处理
int main(void){

    cv::Mat img,img_clone;
    img_clone=img.clone();
    std::string path="CampusGame.mp4";
    cv::VideoCapture cap(path);
    
    GetContours(img_clone,img);
    while(1){
        cap.read(img_clone);        
        cv::cvtColor(img_clone,img_clone,cv::COLOR_BGR2GRAY);
        cv::GaussianBlur(img_clone,img_clone,cv::Size(5,5),3,0);
        //cv::threshold(img_clone, img_clone, 140, 255, cv::THRESH_BINARY);
	    cv::threshold(img_clone, img_clone, 0, 255, cv::THRESH_OTSU);
        cv::Mat kernel=cv::getStructuringElement(cv::MORPH_RECT,cv::Size(5,5));
        cv::erode(img_clone,img_clone,kernel);
        cv::imshow(" ",img_clone);
        cv::waitKey(0);   
    }
    
}


