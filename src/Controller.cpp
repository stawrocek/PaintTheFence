#include "Controller.h"

Controller::Controller(){}
Controller::~Controller(){}

void Controller::createTrackbars(std::string windowName){
    cv::createTrackbar("min Hue", windowName, &minHue, 255);
    cv::createTrackbar("max Hue", windowName, &maxHue, 255);
    cv::createTrackbar("min Saturation", windowName, &minSaturation, 255);
    cv::createTrackbar("max Saturation", windowName, &maxSaturation, 255);
    cv::createTrackbar("min Value", windowName, &minValue, 255);
    cv::createTrackbar("max Value", windowName, &maxValue, 255);

    cv::createTrackbar("confirm", windowName, &confirmVal, 1);
}

void Controller::displayTreshhold(cv::Mat mat, std::string id, int width, int height){
    Mat imgHSV;
    cvtColor(mat, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
    Mat imgThresholded;
    inRange(imgHSV, Scalar(minHue, minSaturation, minValue), Scalar(maxHue, maxSaturation, maxValue), imgThresholded); //Threshold the image
    //morphological opening (remove small objects from the foreground)
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    //morphological closing (fill small holes in the foreground)
    dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    resize(imgThresholded, imgThresholded,Size(width, height));
    cvtColor(imgThresholded, imgThresholded, CV_GRAY2BGRA);
    //std::cout << type2str(imgThresholded.type()) << " " << type2str(mat.type()) << "\n";
    bitwise_and(imgThresholded, mat, imgThresholded);
    imshow(id, imgThresholded);
}

std::pair<int, int> Controller::getPosition(cv::Mat mat)
{
    Mat imgHSV;
    cvtColor(mat, imgHSV, COLOR_BGR2HSV); //Convert the captured frame from BGR to HSV
    Mat imgThresholded;
    inRange(imgHSV, Scalar(minHue, minSaturation, minValue), Scalar(maxHue, maxSaturation, maxValue), imgThresholded); //Threshold the image
    //morphological opening (removes small objects from the foreground)
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    //morphological closing (removes small holes from the foreground)
    dilate( imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    erode(imgThresholded, imgThresholded, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)) );
    //Calculate the moments of the thresholded image
    Moments oMoments = moments(imgThresholded);
    double dM01 = oMoments.m01;
    double dM10 = oMoments.m10;
    double dArea = oMoments.m00;
    if (dArea > 1000)
    {
        int posX = dM10 / dArea;
        int posY = dM01 / dArea;
        return std::make_pair(posX, posY);  //(0,0) -> lewy gorny rog
    }
    return std::make_pair(-1000, -1000);
}
