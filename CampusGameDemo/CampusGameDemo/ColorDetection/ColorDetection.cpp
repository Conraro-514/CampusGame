#include"ColorDetection.h"
using namespace std;
using namespace cv;

double getDistance (cv::Point2f pointO,cv::Point2f pointA )  
{  
    double distance;  
    distance = powf((pointO.x - pointA.x),2) + powf((pointO.y - pointA.y),2);  
    distance = sqrtf(distance); 
    return distance;  
}  

void ColorDetection(cv::Mat img_clone, cv::Mat img){

    vector<Mat> channels;
    split(img_clone, channels);
    Mat thresholdImage,mask;
    bool enemy_color=1;
    
    //目标颜色判断： true红色 false蓝色    通道相减
    if(enemy_color){
        cv::subtract(channels[2],channels[0],img_clone);
    }else{
        cv::subtract(channels[0],channels[2],img_clone);
    }
    
    
    //2值化
    cv::threshold(img_clone, img_clone, 60, 255, cv::THRESH_BINARY);
    
    //滤波
    cv::GaussianBlur(img_clone,img_clone,cv::Size(5,5),0);
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
    cv::dilate(img_clone, img_clone, element);
    
    //框选矩形
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(img_clone, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);//找轮廓
    vector<Rect> boundRect(contours.size());
    for(int i=0;i<contours.size()-1;i++){
        std::vector<cv::Point> points=contours[i];
        int area=contourArea(points);
        boundRect[i]=boundingRect(points);
        int h=boundRect[i].height;
        int w=boundRect[i].width;
        if(area<200) continue;
        cv::Point2f rect[4];
	    cv::RotatedRect box = cv::minAreaRect(cv::Mat(contours[i]));//获取最小外接矩阵
	    // cv::circle(img, cv::Point(box.center.x, box.center.y), 5, cv::Scalar(0, 255, 255), -3, -1);  //绘制最小外接矩形的中心点
	    box.points(rect);  //把最小外接矩形四个端点复制给rect数组
	    for (int j = 0; j < 4; j++){
		cv::line(img, rect[j], rect[(j + 1) % 4], cv::Scalar(0, 255, 0), 2, 8);  //绘制最小外接矩形每条边
	    }
        for(int j=1;j<contours.size();j++){
            std::vector<Point> points1;
            points1 = contours[j]; 
            cv::RotatedRect box1 = cv::minAreaRect(cv::Mat(contours[j]));
            if(abs(box.size.width-box1.size.width)<0.2&&
               abs(box.size.height-box1.size.height)<0.2&&
               abs(getDistance(box.center, box1.center)-box.size.width*3*1.414)<0.2&&
               box.center.x<box1.center.x){
                 cv::Point center=(box1.center+box.center)/2;
                 circle(img,center,5,Scalar(0,0,255),2);
               }
    }

    
    
    
    
    
    cv::imshow("img", img);
    cv::waitKey(1);
  
}