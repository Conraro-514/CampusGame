#include"MindmillAttacter.h"

int LeastSquaresCircleFitting(std::vector<cv::Point2d> &m_Points, 
                              cv::Point2d &Centroid, double &dRadius)//拟合圆函数(三个参数依次为输入点集，圆心，半径)
{
	if (!m_Points.empty())
	{
		int iNum = (int)m_Points.size();
		if (iNum < 3)	return 1;
		double X1 = 0.0;
		double Y1 = 0.0;
		double X2 = 0.0;
		double Y2 = 0.0;
		double X3 = 0.0;
		double Y3 = 0.0;
		double X1Y1 = 0.0;
		double X1Y2 = 0.0;
		double X2Y1 = 0.0;
		std::vector<cv::Point2d>::iterator iter;
		std::vector<cv::Point2d>::iterator end = m_Points.end();
		for (iter = m_Points.begin(); iter != end; ++iter)
		{
			X1 = X1 + (*iter).x;
			Y1 = Y1 + (*iter).y;
			X2 = X2 + (*iter).x * (*iter).x;
			Y2 = Y2 + (*iter).y * (*iter).y;
			X3 = X3 + (*iter).x * (*iter).x * (*iter).x;
			Y3 = Y3 + (*iter).y * (*iter).y * (*iter).y;
			X1Y1 = X1Y1 + (*iter).x * (*iter).y;
			X1Y2 = X1Y2 + (*iter).x * (*iter).y * (*iter).y;
			X2Y1 = X2Y1 + (*iter).x * (*iter).x * (*iter).y;
		}
		double C = 0.0;
		double D = 0.0;
		double E = 0.0;
		double G = 0.0;
		double H = 0.0;
		double a = 0.0;
		double b = 0.0;
		double c = 0.0;
		C = iNum * X2 - X1 * X1;
		D = iNum * X1Y1 - X1 * Y1;
		E = iNum * X3 + iNum * X1Y2 - (X2 + Y2) * X1;
		G = iNum * Y2 - Y1 * Y1;
		H = iNum * X2Y1 + iNum * Y3 - (X2 + Y2) * Y1;
		a = (H * D - E * G) / (C * G - D * D);
		b = (H * C - E * D) / (D * D - G * C);
		c = -(a * X1 + b * Y1 + X2 + Y2) / iNum;
		double A = 0.0;
		double B = 0.0;
		double R = 0.0;
		A = a / (-2);
		B = b / (-2);
		R = double(sqrt(a * a + b * b - 4 * c) / 2);
		Centroid.x = A;
		Centroid.y = B;
		dRadius = R;
		return 0;
	}
	else
		return 1;
	return 0;
}



void MindmillAttacter(cv::Mat img_clone,cv::Mat img){
    //是否开启打大符模式
    bool attack_mode = 1;
    if(!attack_mode) return;
    cv::Mat img_clone_circle=img_clone.clone();
    cv::Mat mask,mask_circle;
    // cv::Scalar lower(113,0,214);
    // cv::Scalar upper(180,255,255);
    // cv::inRange(img_clone,lower,upper,mask);//识别目标
    cv::Scalar lower_circle(101,71,80);
    cv::Scalar upper_circle(176,255,120);
    cv::inRange(img_clone_circle,lower_circle,upper_circle,mask_circle);//拟合圆

    //泡水
    cv::Mat element1 = getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));//设置内核1
    cv::Mat element2 = getStructuringElement(cv::MORPH_RECT, cv::Size(25, 25));//设置内核2
    cv::Mat element3 = getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));//设置内核2
    cv::Mat element4 = getStructuringElement(cv::MORPH_RECT, cv::Size(1, 1));
    // cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, element2);//闭运算
    // cv::floodFill(mask, cv::Point(0, 0), cv::Scalar(0));//漫水法
    cv::morphologyEx(mask_circle, mask_circle, cv::MORPH_OPEN,  element4);//开运算
    cv::morphologyEx(mask_circle, mask_circle, cv::MORPH_CLOSE, element3);//闭运算
    cv::floodFill(mask_circle, cv::Point(0, 0), cv::Scalar(0));//漫水法
	// cv::morphologyEx(mask_circle, mask_circle, cv::MORPH_OPEN,  element1);//开运算
    cv::imshow("mask", mask_circle);
    cv::waitKey(1);
    
    //找轮廓
    // std::vector<std::vector<cv::Point>> contours;
    std::vector<std::vector<cv::Point>> contours_circle;
    // std::vector<cv::Vec4i> hierarchy;
    std::vector<cv::Vec4i> hierarchy_circle;
    // cv::findContours(mask, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);//找轮廓
    cv::findContours(mask_circle, contours_circle, hierarchy_circle, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);//找拟合圆矩形轮廓
    std::vector<cv::Point2d> circle_points;//拟合圆数集
	for(int i = 0; i < contours_circle.size();i++){
        std::vector<cv::Point> points_circle=contours_circle[i];
        int area_circle=contourArea(points_circle);   
        if(area_circle<300||area_circle>800) continue;//通过面积筛选    
        // cv::Point2f rect_circle[4];
	    cv::RotatedRect box_circle = cv::minAreaRect(cv::Mat(contours_circle[i]));//获取最小外接矩阵
	    cv::circle(img, cv::Point(box_circle.center.x, box_circle.center.y), 5, cv::Scalar(0, 255, 255), -3, -1);  //绘制最小外接矩形的中心点
	    // box_circle.points(rect_circle);  //把最小外接矩形四个端点复制给rect数组
	    // for (int j = 0; j < 4; j++){
		// cv::line(img, rect_circle[j], rect_circle[(j + 1) % 4], cv::Scalar(0, 255, 0), 2, 8);  //绘制最小外接矩形每条边
	    // }
        circle_points.push_back(box_circle.center);//储存最小外接矩形中心点
    }
	//拟合圆
	cv::Point2d c;//圆心
    double r = 0;//半径
    LeastSquaresCircleFitting(circle_points, c, r);//拟合圆
    cv::circle(img, c, r, cv::Scalar(0, 0, 255), 2, 8);//绘制圆
    cv::circle(img, c, 5, cv::Scalar(0, 255, 255), -1, 8);//绘制圆心  
      
    

    // for(int i=0;i<contours.size();i++){
    //     std::vector<cv::Point> points=contours[i];
    //     int area=contourArea(points);
        
    //     if(area<200) continue;//通过面积和长宽筛选
    //     cv::Point2f rect[4];
	//     cv::RotatedRect box = cv::minAreaRect(cv::Mat(contours[i]));//获取最小外接矩阵
	//     cv::circle(img, cv::Point(box.center.x, box.center.y), 5, cv::Scalar(0, 255, 255), -3, -1);  //绘制最小外接矩形的中心点
	//     box.points(rect);  //把最小外接矩形四个端点复制给rect数组
	//     for (int j = 0; j < 4; j++){
	// 	cv::line(img, rect[j], rect[(j + 1) % 4], cv::Scalar(0, 255, 0), 2, 8);  //绘制最小外接矩形每条边
	//     }
    // }
    cv::imshow("img", img);
    cv::waitKey(1);
    
   

}