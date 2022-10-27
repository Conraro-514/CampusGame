#include"MindmillAttacter.h"

void MindmillAttacter(cv::Mat img){
    //是否开启打大符模式
    bool attack_mode = 1;
    if(!attack_mode) return;
    cv::Mat mask;
    cv::Scalar lower(156,43,46);
    cv::Scalar upper(180,255,255);
    cv::inRange(img,lower,upper,mask);
    cv::imshow("mask",mask);
    cv::waitKey(1);
}