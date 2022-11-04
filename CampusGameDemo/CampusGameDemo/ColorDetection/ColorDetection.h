#include <stdio.h>  
#include <time.h>  
#include <opencv2/opencv.hpp>    
#include <iostream> 
#include <NetworkManager.h>
#include <algorithm>

double ColorDetection(cv::Mat img_clone,cv::Mat img);

double getDistance (cv::Point2f pointO,cv::Point2f pointA );

bool cmp(cv::Point2f x,cv::Point2f y);

double rotationMatrixToEulerAngles(cv::Mat &R);
