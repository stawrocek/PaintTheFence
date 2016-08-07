#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

#include <iostream>
#include <string>
#include <sstream>
#include <windows.h>

#include "Controller.h"

using namespace cv;

class GameManager
{
    public:
        GameManager();
        virtual ~GameManager(){
            display.release();
        };

        bool openCam();
        void play();
        void config();
        void playGame();
        void postProcessing();

        void printGUI(Mat mat);
        void printText(Mat mat, String str, Point p);
        string doubleToString(double d){
            std::ostringstream s;
            s << d;
            return s.str();
        };
        string type2str(int type) {
          string r;
          uchar depth = type & CV_MAT_DEPTH_MASK;
          uchar chans = 1 + (type >> CV_CN_SHIFT);
          switch ( depth ) {
            case CV_8U:  r = "8U"; break;
            case CV_8S:  r = "8S"; break;
            case CV_16U: r = "16U"; break;
            case CV_16S: r = "16S"; break;
            case CV_32S: r = "32S"; break;
            case CV_32F: r = "32F"; break;
            case CV_64F: r = "64F"; break;
            default:     r = "User"; break;
          }
          r += "C";
          r += (chans+'0');
          return r;
        }

        VideoCapture cam;
        Mat display;
        Mat gameTarget;
        Mat painted;
        string leftPlayer="PLAYER 1";
        string rightPlayer="PLAYER 2";
        double leftPlayerScore=0.0;
        double rightPlayerScore=0.0;
        int maximumScore=0;
        Scalar leftPlayerColor;
        Scalar rightPlayerColor;
        int brushSize = 30;
        int width=640;
        int height=480;
        Controller controllerLeft;
        Controller controllerRight;
        int gameState = 0;     //0->konfig kontrolerow, 1->gra
};

#endif // GAMEMANAGER_HPP
