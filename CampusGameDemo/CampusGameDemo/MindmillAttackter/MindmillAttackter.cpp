#include"MindmillAttackter.h"

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

/////////////   功能实现   //////////////////

cv::Point2f MindmillAttackter(cv::Mat img_clone,cv::Mat img,double &previous_angle){
    
    cv::Mat img_clone_circle=img_clone.clone();
    cv::Mat mask,mask_circle;
    cv::Scalar lower(113,0,214);
    cv::Scalar upper(180,255,255);
    cv::inRange(img_clone,lower,upper,mask);//识别目标
    cv::Scalar lower_circle(0,33,204);
    cv::Scalar upper_circle(179,255,255);
    cv::inRange(img_clone_circle,lower_circle,upper_circle,mask_circle);//圆心

    
    cv::Mat element1 = getStructuringElement(cv::MORPH_RECT, cv::Size(7, 7));//设置内核1
    cv::Mat element2 = getStructuringElement(cv::MORPH_RECT, cv::Size(25, 25));//设置内核2
    cv::Mat element3 = getStructuringElement(cv::MORPH_RECT, cv::Size(15, 15));//设置内核2
    cv::Mat element4 = getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3));
    //目标识别
    cv::morphologyEx(mask, mask, cv::MORPH_CLOSE, element2);//闭运算
    cv::floodFill(mask, cv::Point(0, 0), cv::Scalar(0));//漫水法
    
    //R标识别
    cv::morphologyEx(mask_circle, mask_circle, cv::MORPH_CLOSE, element4);//闭运算
    cv::floodFill(mask_circle, cv::Point(0, 0), cv::Scalar(0));//漫水法
    cv::GaussianBlur(mask_circle,mask_circle,cv::Size(9,9),2,2);
    RemoveBigRegion(mask_circle, mask_circle, 400, 1, 1);
    RemoveSmallRegion(mask_circle, mask_circle, 300, 1, 1);
    cv::threshold(mask_circle, mask_circle, 0, 255, cv::THRESH_OTSU);
    // cv::imshow("img6", mask_circle);
    // cv::waitKey(1);
    
    //找轮廓
    std::vector<std::vector<cv::Point>> contours;
    std::vector<std::vector<cv::Point>> contours_circle;
    std::vector<cv::Vec4i> hierarchy;
    std::vector<cv::Vec4i> hierarchy_circle;
    cv::findContours(mask, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);//找轮廓
    cv::findContours(mask_circle, contours_circle, hierarchy_circle, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);//找拟合圆心轮廓
	
    cv::Point2d c;//圆心
    double r = 80;//半径
    //画外接圆
    for(int i = 0; i < contours_circle.size();i++){
        std::vector<cv::Point> points_circle=contours_circle[i];
        int area_circle=contourArea(points_circle);   
        if(area_circle<100) continue;//通过面积筛选    
        cv::Point2f rect_circle[4];
	    cv::RotatedRect box_circle = cv::minAreaRect(cv::Mat(contours_circle[i]));//获取最小外接矩阵
	    cv::circle(img, cv::Point(box_circle.center.x, box_circle.center.y), 5, cv::Scalar(0, 255, 255), -3, -1);  //绘制最小外接矩形的中心点
	    c = box_circle.center;
        cv::circle(img, cv::Point(box_circle.center.x, box_circle.center.y), r , cv::Scalar(0, 0, 255), 1);  //绘制圆
    }
	
    cv::Point2f resPoint(0,0); //预测击打点
    
    for(int i=0;i<contours.size();i++){
        std::vector<cv::Point> points=contours[i];
        int area=contourArea(points);
        
        if(area<200) continue;//面积筛选
        cv::Point2f rect[4];
	    cv::RotatedRect box = cv::minAreaRect(cv::Mat(contours[i]));//获取最小外接矩阵
	    //cv::circle(img, cv::Point(box.center.x, box.center.y), 5, cv::Scalar(0, 255, 255), -3, -1);  //绘制最小外接矩形的中心点
	    box.points(rect);  //把最小外接矩形四个端点复制给rect数组
	    for (int j = 0; j < 4; j++){
		cv::line(img, rect[j], rect[(j + 1) % 4], cv::Scalar(0, 255, 0), 2, 8);  //绘制最小外接矩形每条边
	    }
        
        //预测
        if(c.x!=0&&c.y!=0){
        cv::Mat rot_mat=cv::getRotationMatrix2D(c,-50,1);
        float sinA=rot_mat.at<double>(0,1);//sin(60);
        float cosA=rot_mat.at<double>(0,0);//cos(60);
        float xx=-(c.x-box.center.x);
        float yy=-(c.y-box.center.y);
        resPoint=cv::Point2f(c.x+cosA*xx-sinA*yy,c.y+sinA*xx+cosA*yy);
        cv::circle(img,resPoint,1,cv::Scalar(0,255,0),10);
        }
        // cv::imshow("img", img);
        // cv::waitKey(1);
        
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
    return resPoint;
}