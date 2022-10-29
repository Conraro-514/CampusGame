#include <stdio.h>  
#include <time.h>  
#include <opencv2/opencv.hpp>    
#include <iostream> 
#include <NetworkManager.h>

cv::Point ColorDetection(cv::Mat img_clone,cv::Mat img);

double getDistance (cv::Point2f pointO,cv::Point2f pointA );