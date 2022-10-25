#include"GetContours.h"

void GetContours(cv::Mat img,cv::Mat img_clone){
    std::vector<vector<Point>> contours;
    std::vector<Vec4i> hierarchy;
    cv::findContours(img_clone,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    std::vector<Rect> boundRect(contours.size());
    for(int i=0;i<contours.size();i++){
        int area=contourArea(contours[i]);
        cv::boundRect[i]=boundingRect(contours[i]);
        int h=boundRect[i].height;
        int w=boundRect[i].width;












    }
}