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
        // cv::imshow(" ",img);
        // cv::waitKey(1);
        MindmillAttacter(img_clone,img,previous_angle);
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
