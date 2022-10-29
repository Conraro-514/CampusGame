#include <stdio.h>  
#include <time.h>  
#include <opencv2/opencv.hpp>    
#include <iostream> 

void ColorDetection(cv::Mat img_clone,cv::Mat img);

double getDistance (cv::Point2f pointO,cv::Point2f pointA );