#include <stdio.h>  
#include <time.h>  
#include <opencv2/opencv.hpp>  
#include <iostream> 
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/ml/ml.hpp>  
#include <io.h>

#include"1/1.h"
//预处理
int main(void){

    cv::Mat img,img_clone;
    img_clone=img.clone();
    std::string path="CampusGame.mp4";
    cv::Mat imgGray,imgHSV,mask,imgDil;
    cv::VideoCapture cap(path);

    int min,max=255;
    cv::namedWindow("Trackbars",(img.size().width,img.size().height));
    cv::createTrackbar("Min","Trackbars",&min,255);
    cv::createTrackbar("Max","Trackbars",&max,255);
    while(1){
        cap.read(img);
        cv::cvtColor(img,imgGray,cv::COLOR_BGR2GRAY);
        cv::threshold(imgGray, imgGray, min, max, cv::THRESH_BINARY);
	    cv::threshold(imgGray, imgGray, 0, 255, cv::THRESH_OTSU);
        cv::imshow(" ",imgGray);
        cv::waitKey(0);   
    }

}


