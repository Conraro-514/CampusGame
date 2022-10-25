#include"GetContours.h"

void GetContours(cv::Mat img,cv::Mat img_clone){
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(img_clone,contours,hierarchy,cv::RETR_EXTERNAL,cv::CHAIN_APPROX_SIMPLE);
    std::vector<cv::Rect> boundRect(contours.size());
    for(int i=0;i<contours.size();i++){
        int area=cv::contourArea(contours[i]);
        boundRect[i]=cv::boundingRect(contours[i]);
        int h=boundRect[i].height;
        int w=boundRect[i].width;












    }
}