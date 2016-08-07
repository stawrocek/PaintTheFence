#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <iostream>
#include <string>
#include <algorithm>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"

using namespace cv;

class Controller
{
    public:
        Controller();
        virtual ~Controller();

        int minHue=0, maxHue=255;
        int minSaturation=0, maxSaturation=255;
        int minValue=0, maxValue=255;
        int confirmVal = 0; //0->false, 1->true, brak kompilacji z qt wiec brak createButton
        void createTrackbars(std::string trackBarWindow);
        void displayTreshhold(cv::Mat mat, std::string id, int width, int height);
        std::pair<int, int> getPosition(cv::Mat mat);


};

#endif // CONTROLLER_H
