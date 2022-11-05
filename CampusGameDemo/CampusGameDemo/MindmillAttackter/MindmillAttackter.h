#include <iostream>
#include <opencv2/opencv.hpp>
#include <time.h>
#include <NetworkManager.h>

cv::Point2f MindmillAttackter(cv::Mat img_clone, cv::Mat img,double &previous_angle);

void RemoveSmallRegion(cv::Mat &Src, cv::Mat &Dst, int AreaLimit, int CheckMode, int NeihborMode);

void RemoveBigRegion(cv::Mat &Src, cv::Mat &Dst, int AreaLimit, int CheckMode, int NeihborMode);

