#include"MindmillAttacter.h"

void MindmillAttacter(cv::Mat img_clone,cv::Mat img){
    //是否开启打大符模式
    bool attack_mode = 1;
    if(!attack_mode) return;
    cv::Mat mask;
    cv::Scalar lower(113,0,214);
    cv::Scalar upper(180,255,255);
    cv::inRange(img_clone,lower,upper,mask);
    //cv::imshow("mask",mask);
    //cv::waitKey(1);

    //泡水
    cv::Mat element1 = getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));//设置内核1
    cv::Mat element2 = getStructuringElement(cv::MORPH_RECT, cv::Size(25, 25));//设置内核2
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, element2);//闭运算
    cv::floodFill(mask, cv::Point(0, 0), cv::Scalar(0));//漫水法
    

    //找轮廓
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(mask, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);//找轮廓
    //cv::drawContours(img,contours,-1,cv::Scalar(0,255,0),10);
    
    for(int i=0;i<contours.size();i++){
        std::vector<cv::Point> points=contours[i];
        int area=contourArea(points);
        //boundRect[i]=boundingRect(points);
        //int h=boundRect[i].height;
        //int w=boundRect[i].width;
        if(area<200) continue;//通过面积和长宽筛选
        cv::Point2f rect[4];
	    cv::RotatedRect box = cv::minAreaRect(cv::Mat(contours[i]));//获取最小外接矩阵
	    cv::circle(img, cv::Point(box.center.x, box.center.y), 5, cv::Scalar(255, 0, 0), -1, 8);  //绘制最小外接矩形的中心点
	    box.points(rect);  //把最小外接矩形四个端点复制给rect数组
	    for (int j = 0; j < 4; j++){
		cv::line(img, rect[j], rect[(j + 1) % 4], cv::Scalar(0, 255, 0), 2, 8);  //绘制最小外接矩形每条边
	}
        cv::imshow("img",img);
        cv::waitKey(1);
    }

}