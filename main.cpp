#include <stdio.h>  
#include <time.h>  
#include <opencv2/opencv.hpp>  
#include <iostream> 
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/ml/ml.hpp>  
#include <io.h>

#include"1/1.h"

int main(){

    cv::Mat img;
    std::string path="CampusGame 2022-10-19 09-16-27.mp4";
    cv::VideoCapture cap(path);
    while(1){
        cap.read(img);
        cv::imshow(" ",img);
        cv::waitKey(0);   
    }
    


return 0;
}


