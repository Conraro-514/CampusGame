#include <NetworkManager.h>
#include <csignal>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>  
#include <time.h>    
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/ml/ml.hpp>  

#include "ColorDetection/ColorDetection.h"
#include"MindmillAttacter/MindmillAttacter.h"
//#include "GetContours/GetContours.h"


bool reg = 0;

void error_handle(int error_id, std::string message);

Net::NetworkManager net("127.0.0.1", "test", 25562, 25564, error_handle);

void sigint_handler(int sig) { exit(1); }

int main() {
    signal(SIGINT, sigint_handler);
    reg = net.registerUser(0);
    while (!reg) {
        std::cout << "Register failed, retrying..." << std::endl;
        reg = net.registerUser(0);
    }
    std::cout << "Register success" << std::endl;
    float yaw =0;
    float pitch = 0; 
    double previous_angle = 0;
    while (true){
        int i = 0;
        while (!reg) {
            std::cout << "Register failed, retrying..." << std::endl;
            reg = net.registerUser(0);
        }
        cv::Mat img;
        img = net.getNewestRecvMessage().img;
        if (!img.empty()) {

  ///////////  My  Code/////////////  
        cv::Mat img_clone = img.clone();
        
        //cv::Point2f PointPre=MindmillAttacter(img_clone,img,previous_angle);
        ColorDetection(img_clone,img);
        

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
        // cv::Mat mask=ColorDetection(img_clone);s

        // cv::imshow("mask", mask);
        // cv::waitKey(1);
        //GetContours(img,mask);
        
        continue;
        } else {
            std::cout << "Get an empty image" << std::endl;
            cv::waitKey(100);
        }
        i++;
    }
    return 0;
}

void error_handle(int error_id, std::string message) {
    if (error_id == 1) {
        reg = 0; 
    }
    std::cout << "Error: " << error_id << " " << message << std::endl;
 }
