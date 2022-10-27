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
    
    
    int hmin=156,smin=43,vmin=46;
    int hmax=180,smax=255,vmax=255;
    cv::Mat HSVmask;
    cv::namedWindow("Trackbars",(640,200));
    cv::createTrackbar("Hue Min","Trackbars",&hmin,179);
    cv::createTrackbar("Hue Max","Trackbars",&hmax,179);
    cv::createTrackbar("Sat Min ","Trackbars",&smin,255);
    cv::createTrackbar("Sat Max","Trackbars",&smax,255);
    cv::createTrackbar("Val Min","Trackbars",&vmin,255);
    cv::createTrackbar("Val Max","Trackbars",&vmax,255);
    
    
    
    while (true) {
        while (!reg) {
            std::cout << "Register failed, retrying..." << std::endl;
            reg = net.registerUser(0);
        }
        cv::Mat img;
        img = net.getNewestRecvMessage().img;
        if (!img.empty()) {

  ///////////  My  Code/////////////  
        cv::Mat img_clone = img.clone();
        //MindmillAttacter(img_clone);
        cv::Scalar lower(hmin,smin,vmin);
        cv::Scalar upper(hmax,smax,vmax);
        cv::inRange(img_clone,lower,upper,HSVmask);
        cv::imshow("HSVmask",HSVmask);
        cv::waitKey(1);
        // cv::Mat mask=ColorDetection(img_clone);
        // cv::imshow("mask", mask);
        // cv::waitKey(1);
        //GetContours(img,mask);
            continue;
        } else {
            std::cout << "Get an empty image" << std::endl;
            cv::waitKey(100);
        }
        


        //net.sendControlMessage(Net::SendStruct(yaw, pitch, 0, 20.0, 0, 0.0, 0.0, -1, -1));
    }
    return 0;
}

void error_handle(int error_id, std::string message) {
    if (error_id == 1) {
        reg = 0;
    }
    std::cout << "Error: " << error_id << " " << message << std::endl;
 }
