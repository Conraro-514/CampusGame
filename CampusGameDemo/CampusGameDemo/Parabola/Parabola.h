#include <NetworkManager.h>
#include <csignal>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>  
#include <time.h>    
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/ml/ml.hpp>  
#include "ColorDetection/ColorDetection.h"
#include "MindmillAttacter/MindmillAttacter.h"

double Parabola(cv::Point2f point, double v);
