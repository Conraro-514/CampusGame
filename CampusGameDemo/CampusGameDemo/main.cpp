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
#include "MindmillAttacter/MindmillAttacter.h"
//#include "GetContours/GetContours.h"


bool reg = 0;

void error_handle(int error_id, std::string message);

Net::NetworkManager net("192.168.1.2", 20214250 ,"超高校级学园爱朵露队", 25562, 25564, error_handle);

void sigint_handler(int sig) { exit(1); }

double change(double a)
{
	if(a>70)return a-360;
	else return a;
}

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
    cv::Point collimation=cv::Point(320,240);
    int i = 0;
    while (true){
        
        int a = net.getNewestRecvMessage().yaw;
		int b = net.getNewestRecvMessage().pitch;
		b = change(b);
    
        // if (a>=330||a<=30) a=180;
        
        while (!reg) {
            std::cout << "Register failed, retrying..." << std::endl;
            reg = net.registerUser(0);
        }
        cv::Mat img;
        img = net.getNewestRecvMessage().img;
        if (!img.empty()) {

  ///////////  My  Code/////////////  
        cv::Mat img_clone = img.clone();
        // cv::imshow(" ",img_clone);
        // cv::waitKey(1);
        cv::Point2f pnt=MindmillAttacter(img_clone,img,previous_angle);
        
        
        ///////////////////////////////打符/////////////////////////////////
        i++;
        if(!pnt.x==0&&!pnt.y==0){
        
        float p1 = fabs((pnt.y - 240)*(13.5/200));
		float y1 = fabs((pnt.x - 320)*(13.5/200));
        
        if(pnt.x>320&&pnt.y>240){
			    a=a+y1;
                b=b+p1;    
			}
		else if(pnt.x<320&&pnt.y<240) {
				a=a-y1;
                b=b-p1;
			}
		else if(pnt.x>320&&pnt.y<240){
				a=a+y1;
                b=b-p1;
			} 
		else{
				a=a-y1;
                b=b+p1;			
			}

        net.sendControlMessage(Net::SendStruct(a,b-5,0,-1, 0, 0, 0, -1, -1));

        if(!(i%5)){
        net.sendControlMessage(Net::SendStruct(a,b-5,1,-1, 0, 0, 0, -1, -1));
        }








        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        
        /////////////////////////////打靶//////////////////////////////
        // i++;
        // cv::Point pnt=ColorDetection(img_clone,img);
        
        // if(pnt.x==320&&pnt.y==240&&!i%10){
        //     a+=30;
        //     b=-7;
        // }
        
        
        // if(!pnt.x==0&&!pnt.y==0){
        
        // float p1 = fabs((pnt.y - 240)*(13.5/200));
		// float y1 = fabs((pnt.x - 320)*(13.5/200));
        
        // net.sendControlMessage(Net::SendStruct(yaw, pitch, 1, 20.0, 0, 0.0, 0.0, -1, -1));
        // if(pnt.x>320&&pnt.y>240){
		// 	    net.sendControlMessage(Net::SendStruct(a+y1,b+p1,0,-1, 0, 0, 0, -1, -1));
		// 	    net.sendControlMessage(Net::SendStruct(a+y1+2,b+p1-7,1,-1, 0, 0, 0, -1, -1));
		// 	}
		// else if(pnt.x<320&&pnt.y<240) {
		// 		net.sendControlMessage(Net::SendStruct(a-y1,b-p1,0,-1, 0, 0, 0, -1, -1));
		// 		net.sendControlMessage(Net::SendStruct(a-y1-2,b-p1-7,1,-1, 0, 0, 0, -1, -1));
		// 	}
		// else if(pnt.x>320&&pnt.y<240){
		// 		net.sendControlMessage(Net::SendStruct(a+y1,b-p1,0,-1, 0, 0, 0, -1, -1));
		// 	    net.sendControlMessage(Net::SendStruct(a+y1+2,b-p1-7,1,-1, 0, 0, 0, -1, -1));
		// 	} 
		// else{
		// 		net.sendControlMessage(Net::SendStruct(a-y1,b+p1,0,-1, 0, 0, 0, -1, -1));
		// 		net.sendControlMessage(Net::SendStruct(a-y1-2,b+p1-7,1,-1, 0, 0, 0, -1, -1));
		// 	}
        // }
        // else {
        // a++,b=-8;
        // net.sendControlMessage(Net::SendStruct(a,b,0,-1, 0, 0, 0, -1, -1));
        // }
        
    
        
        
        
        
        
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
        }
        
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
    if (error_id == 5) {
        reg = net.registerUser(cv::getTickCount()); 
    }
    std::cout << "Error: " << error_id << " " << message << std::endl;
 }
