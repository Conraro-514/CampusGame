#include <iostream>
#include <opencv2/opencv.hpp>

void MindmillAttacter(cv::Mat img_clone, cv::Mat img);

int LeastSquaresCircleFitting(std::vector<cv::Point2d> &m_Points, 
                                cv::Point2d &Centroid, 
                                double &dRadius);//拟合圆函数(三个参数依次为输入点集，
                                                                //圆心，半径)
void RemoveSmallRegion(cv::Mat &Src, cv::Mat &Dst, int AreaLimit, int CheckMode, int NeihborMode);

void RemoveBigRegion(cv::Mat &Src, cv::Mat &Dst, int AreaLimit, int CheckMode, int NeihborMode);