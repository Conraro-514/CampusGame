#include"MindmillAttacter.h"

void pointPrediction(cv::Point circle_center_point,
                          cv::Point target_point,
                          double &previous_angle,double r,double time){
    double target_polar_angle = (180 / 3.14 * atan2((-1 * (target_point.y - circle_center_point.y)), 
                                                          (target_point.x - circle_center_point.x)));
    std::cout<<target_polar_angle<<std::endl;
    
    std::cout<<cv::getTickCount()<<std::endl;
    
}

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

//处理二值图函数
// CheckMode:  0 代表去除黑区域， 1 代表去除白区域; NeihborMode： 0 代表 4 邻域， 1 代表 8 邻域;
void RemoveSmallRegion(cv::Mat &Src, cv::Mat &Dst, int AreaLimit, int CheckMode, int NeihborMode) {
    int RemoveCount = 0;       // 记录除去的个数
    // 记录每个像素点检验状态的标签， 0 代表未检查， 1 代表正在检查， 2 代表检查不合格（需要反转颜色）， 3 代表检查合格或不需检查
    cv::Mat Pointlabel = cv::Mat::zeros(Src.size(), CV_8UC1);

    if (CheckMode == 1) {
//        std::cout << "Mode: 去除小区域. ";
        for (int i = 0; i < Src.rows; ++i) {
            uchar *iData = Src.ptr<uchar>(i);
            uchar *iLabel = Pointlabel.ptr<uchar>(i);
            for (int j = 0; j < Src.cols; ++j) {
                if (iData[j] < 10) {
                    iLabel[j] = 3;
                }
            }
        }
    } else {
//        std::cout << "Mode: 去除孔洞. ";
        for (int i = 0; i < Src.rows; ++i) {
            uchar *iData = Src.ptr<uchar>(i);
            uchar *iLabel = Pointlabel.ptr<uchar>(i);
            for (int j = 0; j < Src.cols; ++j) {
                if (iData[j] > 10) {
                    iLabel[j] = 3;
                }
            }
        }
    }

    std::vector<cv::Point2i> NeihborPos;  // 记录邻域点位置
    NeihborPos.push_back(cv::Point2i(-1, 0));
    NeihborPos.push_back(cv::Point2i(1, 0));
    NeihborPos.push_back(cv::Point2i(0, -1));
    NeihborPos.push_back(cv::Point2i(0, 1));
    if (NeihborMode == 1) {
//        std::cout << "Neighbor mode: 8 邻域." << std::endl;
        NeihborPos.push_back(cv::Point2i(-1, -1));
        NeihborPos.push_back(cv::Point2i(-1, 1));
        NeihborPos.push_back(cv::Point2i(1, -1));
        NeihborPos.push_back(cv::Point2i(1, 1));
    }
    // else std::cout << "Neighbor mode: 4 邻域." << std::endl;
    int NeihborCount = 4 + 4 * NeihborMode;
    int CurrX = 0, CurrY = 0;
    // 开始检测
    for (int i = 0; i < Src.rows; ++i) {
        uchar *iLabel = Pointlabel.ptr<uchar>(i);
        for (int j = 0; j < Src.cols; ++j) {
            if (iLabel[j] == 0) {
                //********开始该点处的检查**********
                std::vector<cv::Point2i> GrowBuffer;  // 堆栈，用于存储生长点
                GrowBuffer.push_back(cv::Point2i(j, i));
                Pointlabel.at<uchar>(i, j) = 1;
                int CheckResult = 0;  // 用于判断结果（是否超出大小），0为未超出，1为超出

                for (int z = 0; z < GrowBuffer.size(); z++) {

                    for (int q = 0; q < NeihborCount; q++)  //检查四个邻域点
                    {
                        CurrX = GrowBuffer.at(z).x + NeihborPos.at(q).x;
                        CurrY = GrowBuffer.at(z).y + NeihborPos.at(q).y;
                        if (CurrX >= 0 && CurrX < Src.cols && CurrY >= 0 && CurrY < Src.rows)  // 防止越界
                        {
                            if (Pointlabel.at<uchar>(CurrY, CurrX) == 0) {
                                GrowBuffer.push_back(cv::Point2i(CurrX, CurrY));  // 邻域点加入buffer
                                Pointlabel.at<uchar>(CurrY, CurrX) = 1;  // 更新邻域点的检查标签，避免重复检查
                            }
                        }
                    }

                }
                if (GrowBuffer.size() > AreaLimit) CheckResult = 2;  //判断结果（是否超出限定的大小），1为未超出，2为超出
                else {
                    CheckResult = 1;
                    RemoveCount++;
                }
                for (int z = 0; z < GrowBuffer.size(); z++)  //更新Label记录
                {
                    CurrX = GrowBuffer.at(z).x;
                    CurrY = GrowBuffer.at(z).y;
                    Pointlabel.at<uchar>(CurrY, CurrX) += CheckResult;
                }
                //********结束该点处的检查**********


            }
        }
    }

    CheckMode = 255 * (1 - CheckMode);
    //开始反转面积过小的区域
    for (int i = 0; i < Src.rows; ++i) {
        uchar *iData = Src.ptr<uchar>(i);
        uchar *iDstData = Dst.ptr<uchar>(i);
        uchar *iLabel = Pointlabel.ptr<uchar>(i);
        for (int j = 0; j < Src.cols; ++j) {
            if (iLabel[j] == 2) {
                iDstData[j] = CheckMode;
            } else if (iLabel[j] == 3) {
                iDstData[j] = iData[j];
            }
        }
    }

//    std::cout << RemoveCount << " objects removed." << std::endl;
}

// CheckMode:  0 代表去除黑区域， 1 代表去除白区域; NeihborMode： 0 代表 4 邻域， 1 代表 8 邻域;
void RemoveBigRegion(cv::Mat &Src, cv::Mat &Dst, int AreaLimit, int CheckMode, int NeihborMode) {
    int RemoveCount = 0;       // 记录除去的个数
    // 记录每个像素点检验状态的标签， 0 代表未检查， 1 代表正在检查， 2 代表检查不合格（需要反转颜色）， 3 代表检查合格或不需检查
    cv::Mat Pointlabel = cv::Mat::zeros(Src.size(), CV_8UC1);

    if (CheckMode == 1) {
//        std::cout << "Mode: 去除小区域. ";
        for (int i = 0; i < Src.rows; ++i) {
            uchar *iData = Src.ptr<uchar>(i);
            uchar *iLabel = Pointlabel.ptr<uchar>(i);
            for (int j = 0; j < Src.cols; ++j) {
                if (iData[j] < 10) {
                    iLabel[j] = 3;
                }
            }
        }
    } else {
//        std::cout << "Mode: 去除孔洞. ";
        for (int i = 0; i < Src.rows; ++i) {
            uchar *iData = Src.ptr<uchar>(i);
            uchar *iLabel = Pointlabel.ptr<uchar>(i);
            for (int j = 0; j < Src.cols; ++j) {
                if (iData[j] > 10) {
                    iLabel[j] = 3;
                }
            }
        }
    }

    std::vector<cv::Point2i> NeihborPos;  // 记录邻域点位置
    NeihborPos.push_back(cv::Point2i(-1, 0));
    NeihborPos.push_back(cv::Point2i(1, 0));
    NeihborPos.push_back(cv::Point2i(0, -1));
    NeihborPos.push_back(cv::Point2i(0, 1));
    if (NeihborMode == 1) {
//        std::cout << "Neighbor mode: 8 邻域." << std::endl;
        NeihborPos.push_back(cv::Point2i(-1, -1));
        NeihborPos.push_back(cv::Point2i(-1, 1));
        NeihborPos.push_back(cv::Point2i(1, -1));
        NeihborPos.push_back(cv::Point2i(1, 1));
    }
    // else std::cout << "Neighbor mode: 4 邻域." << std::endl;
    int NeihborCount = 4 + 4 * NeihborMode;
    int CurrX = 0, CurrY = 0;
    // 开始检测
    for (int i = 0; i < Src.rows; ++i) {
        uchar *iLabel = Pointlabel.ptr<uchar>(i);
        for (int j = 0; j < Src.cols; ++j) {
            if (iLabel[j] == 0) {
                //********开始该点处的检查**********
                std::vector<cv::Point2i> GrowBuffer;  // 堆栈，用于存储生长点
                GrowBuffer.push_back(cv::Point2i(j, i));
                Pointlabel.at<uchar>(i, j) = 1;
                int CheckResult = 0;  // 用于判断结果（是否超出大小），0为未超出，1为超出

                for (int z = 0; z < GrowBuffer.size(); z++) {

                    for (int q = 0; q < NeihborCount; q++)  //检查四个邻域点
                    {
                        CurrX = GrowBuffer.at(z).x + NeihborPos.at(q).x;
                        CurrY = GrowBuffer.at(z).y + NeihborPos.at(q).y;
                        if (CurrX >= 0 && CurrX < Src.cols && CurrY >= 0 && CurrY < Src.rows)  // 防止越界
                        {
                            if (Pointlabel.at<uchar>(CurrY, CurrX) == 0) {
                                GrowBuffer.push_back(cv::Point2i(CurrX, CurrY));  // 邻域点加入buffer
                                Pointlabel.at<uchar>(CurrY, CurrX) = 1;  // 更新邻域点的检查标签，避免重复检查
                            }
                        }
                    }

                }
                if (GrowBuffer.size() < AreaLimit) CheckResult = 2;  //判断结果（是否超出限定的大小），1为未超出，2为超出
                else {
                    CheckResult = 1;
                    RemoveCount++;
                }
                for (int z = 0; z < GrowBuffer.size(); z++)  //更新Label记录
                {
                    CurrX = GrowBuffer.at(z).x;
                    CurrY = GrowBuffer.at(z).y;
                    Pointlabel.at<uchar>(CurrY, CurrX) += CheckResult;
                }
                //********结束该点处的检查**********


            }
        }
    }

    CheckMode = 255 * (1 - CheckMode);
    //开始反转面积过小的区域
    for (int i = 0; i < Src.rows; ++i) {
        uchar *iData = Src.ptr<uchar>(i);
        uchar *iDstData = Dst.ptr<uchar>(i);
        uchar *iLabel = Pointlabel.ptr<uchar>(i);
        for (int j = 0; j < Src.cols; ++j) {
            if (iLabel[j] == 2) {
                iDstData[j] = CheckMode;
            } else if (iLabel[j] == 3) {
                iDstData[j] = iData[j];
            }
        }
    }

//    std::cout << RemoveCount << " objects removed." << std::endl;
}


cv::Point2f MindmillAttacter(cv::Mat img_clone,cv::Mat img,double &previous_angle){
    //是否开启打大符模式
    bool attack_mode = 0;
    if(!attack_mode) return cv::Point2f(-1,-1);
    cv::Mat img_clone_circle=img_clone.clone();
    cv::Mat mask,mask_circle;
    cv::Scalar lower(113,0,214);
    cv::Scalar upper(180,255,255);
    cv::inRange(img_clone,lower,upper,mask);//识别目标
    cv::Scalar lower_circle(111,67,0);
    cv::Scalar upper_circle(179,230,255);
    cv::inRange(img_clone_circle,lower_circle,upper_circle,mask_circle);//拟合圆

    //泡水
    cv::Mat element1 = getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));//设置内核1
    cv::Mat element2 = getStructuringElement(cv::MORPH_RECT, cv::Size(25, 25));//设置内核2
    cv::Mat element3 = getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));//设置内核2
    cv::Mat element4 = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, element2);//闭运算
    cv::floodFill(mask, cv::Point(0, 0), cv::Scalar(0));//漫水法
    
    
    cv::morphologyEx(mask_circle, mask_circle, cv::MORPH_CLOSE, element3);//闭运算
    cv::morphologyEx(mask_circle, mask_circle, cv::MORPH_OPEN,  element4);//开运算
    cv::floodFill(mask_circle, cv::Point(0, 0), cv::Scalar(0));//漫水法
    cv::imshow("mask", mask_circle);
    cv::waitKey(1);
    RemoveSmallRegion(mask_circle, mask_circle, 200, 1, 1);
    // CheckMode:  0 代表去除黑区域， 1 代表去除白区域; NeihborMode： 0 代表 4 邻域， 1 代表 8 邻域;
    RemoveBigRegion(mask_circle, mask_circle, 800, 1, 1);
    
    
    
    
    //cv::morphologyEx(mask_circle, mask_circle, cv::MORPH_OPEN,  element1);//开运算
    
    
    //找轮廓
    std::vector<std::vector<cv::Point>> contours;
    std::vector<std::vector<cv::Point>> contours_circle;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<cv::Vec4i> hierarchy_circle;
    cv::findContours(mask, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);//找轮廓
    cv::findContours(mask_circle, contours_circle, hierarchy_circle, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);//找拟合圆矩形轮廓
    std::vector<cv::Point2d> circle_points;//拟合圆数集
	for(int i = 0; i < contours_circle.size();i++){
        std::vector<cv::Point> points_circle=contours_circle[i];
        int area_circle=contourArea(points_circle);   
        if(area_circle<200||area_circle>600) continue;//通过面积筛选    
        cv::Point2f rect_circle[4];
	    cv::RotatedRect box_circle = cv::minAreaRect(cv::Mat(contours_circle[i]));//获取最小外接矩阵
	    //cv::circle(img, cv::Point(box_circle.center.x, box_circle.center.y), 5, cv::Scalar(0, 255, 255), -3, -1);  //绘制最小外接矩形的中心点
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

    cv::Point2f resPoint(0,0); 
    for(int i=0;i<contours.size();i++){
        std::vector<cv::Point> points=contours[i];
        int area=contourArea(points);
        
        if(area<200) continue;//通过面积和长宽筛选
        cv::Point2f rect[4];
	    cv::RotatedRect box = cv::minAreaRect(cv::Mat(contours[i]));//获取最小外接矩阵
	    cv::circle(img, cv::Point(box.center.x, box.center.y), 5, cv::Scalar(0, 255, 255), -3, -1);  //绘制最小外接矩形的中心点
	    box.points(rect);  //把最小外接矩形四个端点复制给rect数组
	    for (int j = 0; j < 4; j++){
		cv::line(img, rect[j], rect[(j + 1) % 4], cv::Scalar(0, 255, 0), 2, 8);  //绘制最小外接矩形每条边
	    }

        //预测
        if(c.x!=0&&c.y!=0){
        cv::Mat rot_mat=cv::getRotationMatrix2D(c,-90,1);
        float sinA=rot_mat.at<double>(0,1);//sin(60);
        float cosA=rot_mat.at<double>(0,0);//cos(60);
        float xx=-(c.x-box.center.x);
        float yy=-(c.y-box.center.y);
        resPoint=cv::Point2f(c.x+cosA*xx-sinA*yy,c.y+sinA*xx+cosA*yy);
        cv::circle(img,resPoint,1,cv::Scalar(0,255,0),10);
        }
        // double time=400;
        //cv::Point predict_point(0,0);
        //pointPrediction(c,box.center,previous_angle,r,time);
        //cv::circle(img, predict_point, 5, cv::Scalar(0, 0, 255), -1, 8);//绘制圆心
        
    }
    //大符
        //像素坐标和欧拉角转换
        // double fx=941.608;
        // double fy=943.231;
        // double thetaX=atan((PointPre.x-img.cols/2)/(7/fx));
        // double thetaY=atan((PointPre.y-img.rows/2)/(7/fx));
        // std::cout<<PointPre.x<<" "<<PointPre.y<<std::endl;
        // std::cout << img.cols<<" "<< img.rows<<std::endl;
        // std::cout << thetaX<<" "<< thetaY<<std::endl;
        
        // if(PointPre!=cv::Point2f(-1,-1)&&
        //    PointPre.x!=0&&
        //    PointPre.y!=0&&
        //    !i%300){
        // std::cout<<PointPre.x<<" "<<PointPre.y<<std::endl;
        // std::cout << thetaX<<" "<< thetaY<<std::endl;
        // std::cout << img.cols<<" "<< img.rows<<std::endl;
        // net.sendControlMessage(Net::SendStruct(yaw+thetaX, pitch-thetaY, 1, 20.0, 0, 0.0, 0.0, -1, -1));
        // }
    cv::imshow("img", img);
    cv::waitKey(1);
}