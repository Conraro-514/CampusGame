#include <NetworkManager.h>
#include <csignal>
#include <iostream>
#include <opencv2/opencv.hpp>
#include <stdio.h>  
#include <time.h>    
#include <opencv2/core/core.hpp>  
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/ml/ml.hpp>  
#include <opencv2/video/tracking.hpp> 

#include "ColorDetection/ColorDetection.h"
#include "MindmillAttacter/MindmillAttacter.h"

                        
bool reg = 0;

void error_handle(int error_id, std::string message);

Net::NetworkManager net("192.168.1.3", 20214250 ,"超高校级学园爱朵露队", 25562, 25564, error_handle);

void sigint_handler(int sig) { exit(1); }

const int winHeight=640;
const int winWidth=480;

double change(double a)
{
	if(a>70)return a-360;
	else return a;
}

int main() {
    //KalmanFilter
    cv::RNG rng;
	//1.kalman filter setup
	const int stateNum=4;                                      //状态值4×1向量(x,y,△x,△y)
	const int measureNum=2;                                    //测量值2×1向量(x,y)	
	cv::KalmanFilter KF(stateNum, measureNum, 0);	
 
	KF.transitionMatrix = (cv::Mat_<float>(4, 4) <<1,0,1,0,
                                                   0,1,0,1,
                                                   0,0,1,0,
                                                   0,0,0,1);  //转移矩阵A
	setIdentity(KF.measurementMatrix);                                             //测量矩阵H
	setIdentity(KF.processNoiseCov, cv::Scalar::all(1e-5));                            //系统噪声方差矩阵Q
	setIdentity(KF.measurementNoiseCov, cv::Scalar::all(1e-1));                        //测量噪声方差矩阵R
	setIdentity(KF.errorCovPost, cv::Scalar::all(1));                                  //后验错误估计协方差矩阵P
	rng.fill(KF.statePost,cv::RNG::UNIFORM,0,winHeight>winWidth?winWidth:winHeight);   //初始状态值x(0)
	cv::Mat measurement = cv::Mat::zeros(measureNum, 1, CV_32F);                           //初始测量值x'(0)，因为后面要更新这个值，所以必须先定义

    
    ///////////////////////////////
    signal(SIGINT, sigint_handler);
    reg = net.registerUser(cv::getTickCount());
    while (!reg) {
        std::cout << "Register failed, retrying..." << std::endl;
        reg = net.registerUser(cv::getTickCount());
    }
    std::cout << "Register success" << std::endl;
    float yaw =0;
    float pitch = 0; 
    double previous_angle = 0;
    cv::Point collimation=cv::Point(320,240);
    int i = 0;
    while (true){
        int timeRest=net.getNewestRecvMessage().rest_time;
        int timeBuff=net.getNewestRecvMessage().buff_over_time;
        int a = net.getNewestRecvMessage().yaw;
		int b = net.getNewestRecvMessage().pitch;
		b = change(b);
    
        // if (a>=330||a<=30) a=180;
        
        while (!reg) {
            std::cout << "Register failed, retrying..." << std::endl;
            reg = net.registerUser(cv::getTickCount());
        }
        cv::Mat img;
        img = net.getNewestRecvMessage().img;
        if (!img.empty()) {

  ///////////  My  Code/////////////  
        cv::Mat img_clone = img.clone();
        // cv::imshow(" ",img_clone);
        // cv::waitKey(1);
        
        
        
        ///////////////////////////////打符/////////////////////////////////
        if(timeRest>100){
        cv::Point2f pnt=MindmillAttacter(img_clone,img,previous_angle);

        cv::Mat prediction = KF.predict();
		cv::Point predict_pt = cv::Point(prediction.at<float>(0),prediction.at<float>(1) );   //预测值(x',y')
			//3.update measurement
		measurement.at<float>(0) = (float)pnt.x;
		measurement.at<float>(1) = (float)pnt.y;
			//4.update
		KF.correct(measurement);
			//draw
		// cv::circle(img,predict_pt,5,cv::Scalar(0,0,255),3);    //predicted point with green
		// cv::circle(img,pnt,5,cv::Scalar(255,0,0),3); //current position with red
        
        
        
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

        if(!(i%6)){
        net.sendControlMessage(Net::SendStruct(a,b-5,1,-1, 0, 0, 0, -1, -1));
        }
        }
        }

/////////////////////////////打靶//////////////////////////////
        if(timeRest<100||timeRest>100&&timeBuff>0){
        i++;
        cv::Point pnt=ColorDetection(img_clone,img);
        
        if(pnt.x==320&&pnt.y==240&&!i%10){
            a+=30;
            b=-7;
        }
        
        
        if(!pnt.x==0&&!pnt.y==0){
        
        float p1 = fabs((pnt.y - 240)*(13.5/200));
		float y1 = fabs((pnt.x - 320)*(13.5/200));
        
        net.sendControlMessage(Net::SendStruct(yaw, pitch, 1, 20.0, 0, 0.0, 0.0, -1, -1));
        if(pnt.x>320&&pnt.y>240){
			    net.sendControlMessage(Net::SendStruct(a+y1,b+p1,0,-1, 0, 0, 0, -1, -1));
			    net.sendControlMessage(Net::SendStruct(a+y1+2,b+p1-7,1,-1, 0, 0, 0, -1, -1));
			}
		else if(pnt.x<320&&pnt.y<240) {
				net.sendControlMessage(Net::SendStruct(a-y1,b-p1,0,-1, 0, 0, 0, -1, -1));
				net.sendControlMessage(Net::SendStruct(a-y1-2,b-p1-7,1,-1, 0, 0, 0, -1, -1));
			}
		else if(pnt.x>320&&pnt.y<240){
				net.sendControlMessage(Net::SendStruct(a+y1,b-p1,0,-1, 0, 0, 0, -1, -1));
			    net.sendControlMessage(Net::SendStruct(a+y1+2,b-p1-7,1,-1, 0, 0, 0, -1, -1));
			} 
		else{
				net.sendControlMessage(Net::SendStruct(a-y1,b+p1,0,-1, 0, 0, 0, -1, -1));
				net.sendControlMessage(Net::SendStruct(a-y1-2,b+p1-7,1,-1, 0, 0, 0, -1, -1));
			}
        }
        else {
        a++,b=-8;
        net.sendControlMessage(Net::SendStruct(a,b,0,-1, 0, 0, 0, -1, -1));
        }
        
    
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
