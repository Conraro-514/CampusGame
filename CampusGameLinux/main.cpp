#include <stdio.h>  
#include <time.h>  
#include <opencv2/opencv.hpp>  
#include <iostream> 
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/ml/ml.hpp>  
//#include <io.h>

#include"GetContours/GetContours.h"
#include"ColorDetection/ColorDetection.h"
//预处理
int main(void){

    cv::Mat img,img_clone;
    std::string path="CampusGame.mp4";
    cv::VideoCapture cap(path);
    while(1){
        cap.read(img); 
        img_clone=img.clone();       
        ColorDetcetion(img_clone,0);
        cv::waitKey(0);   
    }
    
}


