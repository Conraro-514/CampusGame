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


bool cmp(cv::Point2f x,cv::Point2f y){
	if(x.x<y.x&&x.y<y.y){
        return true;
    }
    else if(x.x<y.x&&x.y>y.y){
        return true;
    }
    else{
        return false;
    } 
}


/**
 * 功能： 通过给定的旋转矩阵计算对应的欧拉角**/
double rotationMatrixToEulerAngles(cv::Mat &T)
{ 
 
    double dYaw= atan2(T.at<double>(0,0),T.at<double>(2,0));
    double dPitch= atan(T.at<double>(1,0)/(double)sqrt((T.at<double>(0,0)*T.at<double>(0,0))+(T.at<double>(2,0)*T.at<double>(2,0))));
    return dYaw,dPitch;
}
cv::Point2f ColorDetection(cv::Mat img_clone, cv::Mat img){

    vector<Mat> channels;
    split(img_clone, channels);
    Mat thresholdImage,mask;
    bool enemy_color=1;
    double dYaw,dPitch;

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
    std::vector<std::vector<cv::Point>> lightInfos;
    std::vector<cv::Vec4i> hierarchy;
    cv::findContours(img_clone, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);//找轮廓
    vector<Rect> boundRect(contours.size());
    for(int i=0;i<contours.size();i++){
        int area=contourArea(contours[i]);
        boundRect[i]=boundingRect(contours[i]);
        int h=boundRect[i].height;
        int w=boundRect[i].width;
        if(area<500) continue;
        cv::Point2f rect[4];
	    cv::RotatedRect box = cv::minAreaRect(cv::Mat(contours[i]));//获取最小外接矩阵
	    // cv::circle(img, cv::Point(box.center.x, box.center.y), 5, cv::Scalar(0, 255, 255), -3, -1);  //绘制最小外接矩形的中心点
        if(abs(box.size.height/box.size.width-1)>0.3) continue;
	    box.points(rect);  //把最小外接矩形四个端点复制给rect数组
	    // for (int j = 0; j < 4; j++){
		// cv::line(img, rect[j], rect[(j + 1) % 4], cv::Scalar(0, 255, 0), 2, 8);  //绘制最小外接矩形每条边
	    // }
        lightInfos.push_back(contours[i]);//符合条件的存起来
    }
    
    cv::Point2f circleYes;

    for(int i=0;i<lightInfos.size();i++){
            for(int j=0;j<lightInfos.size();j++){
            cv::RotatedRect boxL = cv::minAreaRect(cv::Mat(lightInfos[i]));
            cv::RotatedRect boxR = cv::minAreaRect(cv::Mat(lightInfos[j]));
            double meanLength=(boxL.size.width+boxR.size.width)/2;
            if( //确保两方块在同一矩形上
                abs(boxL.size.width-boxR.size.width)<9&&
                abs(boxL.size.height-boxR.size.height)<9&&
                abs(boxL.angle-boxR.angle)<1&&
                meanLength*1.2<abs(getDistance(boxL.center,boxR.center))<meanLength*5&&
                //匹配左上和右下            
                abs(boxL.center.x-boxR.center.x)>meanLength&&
                abs(boxL.center.y-boxR.center.y)>meanLength){
                //cv::line(img, boxL.center, boxR.center, cv::Scalar(0, 255, 0), 2, 8);
                cv::Point2f center=(boxL.center+boxR.center)/2;
                //circle(img,center,5,Scalar(0,255,255),2);
                circleYes.x=center.x; 
                circleYes.y=center.y;     

                // //圈出目标以进行pnp操作
                // cv::Point2f targetRect[4];
                // cv::RotatedRect target(circleYes, 
                //                    cv::Size2f(2*abs(boxL.center.x-circleYes.x), 
                //                               2*abs(boxR.center.y-circleYes.y)), 
                //                        boxL.angle);
                // target.points(targetRect);
                // //对targetRect进行排序
                // sort(targetRect,targetRect+4,cmp);
                
                // //像素坐标
                // std::vector<cv::Point2d> image_points; 
                // image_points.push_back(Point2d(targetRect[0].x, targetRect[0].y));
                // image_points.push_back(Point2d(targetRect[1].x, targetRect[1].y));
                // image_points.push_back(Point2d(targetRect[2].x, targetRect[2].y));
                // image_points.push_back(Point2d(targetRect[3].x, targetRect[3].y));  

                // //物体坐标
                // std::vector<Point3d> model_points;
                // model_points.push_back(Point3d(-37.5f, +37.5f, 0));
                // model_points.push_back(Point3d(-37.5f, -37.5f, 0));
                // model_points.push_back(Point3d(+37.5f, +37.5f, 0));
                // model_points.push_back(Point3d(+37.5f, -37.5f, 0));      

                // // 相机内参矩阵和畸变系数均由相机标定结果得出
                // // 相机内参矩阵
                // cv::Mat camera_matrix = (cv::Mat_<double>(3, 3) << 944.1089101158454, 0, 294.7671490601489,
                //                                            0, 941.1114167197278, 238.9584637813869,
                //                                            0, 0, 1);
                // // 相机畸变系数
                // cv::Mat dist_coeffs = (cv::Mat_<double>(5, 1) << -0.391591353380317, 8.893477113844746, 
                //                        -0.01789912573596753, -0.01602177447045374, -182.4064851685259);
                
                // // 求解旋转和平移矩阵
                //     //旋转向量
                //     cv::Mat rotation_vector;
                //     //平移向量
                //     cv::Mat translation_vector;
                
                // // pnp求解
                // solvePnP(model_points, image_points, camera_matrix, dist_coeffs, 
                //          rotation_vector, translation_vector);
                // // 默认ITERATIVE方法，可尝试修改为EPNP（CV_EPNP）,P3P（CV_P3P）
                
                // //旋转向量转成旋转矩阵
                // Mat Rvec;
                // Mat_<float> Tvec;
                // rotation_vector.convertTo(Rvec, CV_32F);  // 旋转向量转换格式
                // translation_vector.convertTo(Tvec, CV_32F); // 平移向量转换格式 
                // Mat_<float> rotMat(3, 3);
                // Rodrigues(Rvec, rotMat);//罗德里格斯变换
                // std::cout<<"Rvec:"<<Rvec<<std::endl;
                // std::cout<<"Tvec:"<<Tvec<<std::endl; 
                // dYaw,dPitch=rotationMatrixToEulerAngles(rotMat);
                // std::cout<<"dyaw="<<dYaw<<"  "<<"dpitch="<<dPitch<<std::endl;
                // // rotation_vector.release();
                // // translation_vector.release();    
                
                }             
               }    
    }


    
    cv::imshow("img", img);
    cv::waitKey(1);
    return circleYes;
}