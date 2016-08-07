#include "GameManager.hpp"

GameManager::GameManager(){
    cam.set(CV_CAP_PROP_FRAME_WIDTH,width);
    cam.set(CV_CAP_PROP_FRAME_HEIGHT,height);

    gameTarget = imread("fence.png", CV_LOAD_IMAGE_UNCHANGED);
    resize(gameTarget,gameTarget,Size(width, height));
    //cvtColor(gameTarget, gameTarget, CV_BGR2BGRA);
    gameTarget.convertTo(gameTarget, CV_8UC3);

    for( int y = 0; y < gameTarget.rows; y++ ){
        for( int x = 0; x < gameTarget.cols; x++ ){
            if(gameTarget.at<Vec3b>(y,x)[0] == 255 && gameTarget.at<Vec3b>(y,x)[1] == 255 && gameTarget.at<Vec3b>(y,x)[2] == 255){
                maximumScore++;
            }
        }
    }

    painted = imread("fence.png", CV_LOAD_IMAGE_UNCHANGED);
    resize(painted,painted,Size(width, height));
    //cvtColor(painted, painted, CV_BGR2BGRA);
    painted.convertTo(painted, CV_8UC3);
    painted = Scalar(0,0,0);

    int off = 15;
    namedWindow("Controllers [Player 1]", CV_WINDOW_AUTOSIZE);
    resizeWindow("Controllers [Player 1]", 250, 350);
    moveWindow("Controllers [Player 1]", off, off);
    namedWindow("Controllers [Player 2]", CV_WINDOW_AUTOSIZE);
    resizeWindow("Controllers [Player 2]", 250, 350);
    moveWindow("Controllers [Player 2]", GetSystemMetrics(SM_CXSCREEN)-250-off, off);

    controllerLeft.createTrackbars("Controllers [Player 1]");
    controllerRight.createTrackbars("Controllers [Player 2]");

    leftPlayerColor = Scalar(255, 0, 0);
    rightPlayerColor = Scalar(0, 0, 255);
}

bool GameManager::openCam(){
    cam.open(0);
    if(!cam.isOpened()){
        printf("Nie udalo sie zawiazac polaczenia z kamera!\n");
        return false;
    }
    else {
        printf("Otwieram kamere!\n");
        return true;
    }
}

void GameManager::postProcessing(){

}

void GameManager::printText(Mat mat, String txt, Point location){
    putText(mat, txt, location, CV_FONT_HERSHEY_SIMPLEX, 1, cvScalar(0, 255, 0, 0),3);
}

void GameManager::printGUI(Mat mat){
    printText(mat, leftPlayer, Point(0, 30));
    printText(mat, doubleToString(leftPlayerScore)+"%", Point(0, 60));
    printText(mat, rightPlayer, Point(width-20*rightPlayer.size(), 30));
    printText(mat, doubleToString(rightPlayerScore)+"%", Point(width-60, 60));
}

void GameManager::config(){
    cam >> display;
    flip(display, display, 1);
    resize(display, display, Size(width, height));
    display.convertTo(display, CV_8UC3);
    cvtColor(display, display, CV_BGR2BGRA);
    imshow("thE Fence",display);

    controllerLeft.displayTreshhold(display, "treshold 1", width, height);
    controllerRight.displayTreshhold(display, "treshold 2", width, height);

    //std::cout << controllerLeft.getPosition(display).first << " " << controllerLeft.getPosition(display).second << "\n";
}

void GameManager::playGame(){
    cam >> display;
    flip(display, display, 1);
    resize(display, display, Size(width, height));
    display.convertTo(display, CV_8UC3);
    cvtColor(display, display, CV_BGRA2BGR);
    //cvtColor(display, display, CV_BGR2BGR);

    int leftPosX = controllerLeft.getPosition(display).first;
    int rightPosX = controllerRight.getPosition(display).first;
    int leftPosY = controllerLeft.getPosition(display).second;
    int rightPosY = controllerRight.getPosition(display).second;

    circle(painted, Point(leftPosX, leftPosY), brushSize, leftPlayerColor, -1);
    circle(painted, Point(rightPosX, rightPosY), brushSize, rightPlayerColor, -1);

    bitwise_or(display, gameTarget, display);
    int leftCtr=0, rightCtr=0;
    for( int y = 0; y < gameTarget.rows; y++ ){
        for( int x = 0; x < gameTarget.cols; x++ ){
            if(painted.at<Vec3b>(y,x)[0] != 0 && display.at<Vec3b>(y,x)[0]==255){
                display.at<Vec3b>(y,x) = painted.at<Vec3b>(y,x);
                leftCtr++;
            }
            if(painted.at<Vec3b>(y,x)[2] != 0 && display.at<Vec3b>(y,x)[2]==255){
                display.at<Vec3b>(y,x) = painted.at<Vec3b>(y,x);
                rightCtr++;
            }
        }
    }
    //std::cout << maximumScore << " " << leftCtr << " " << rightCtr << "\n";
    leftCtr *= 100;
    rightCtr *=100;
    leftPlayerScore = leftCtr/(maximumScore);
    rightPlayerScore = rightCtr/(maximumScore);

    //std::cout << type2str(display.type()) << " " << type2str(painted.type()) << "\n";
    printGUI(display);
    imshow("thE Fence",display);
}

void GameManager::play(){
    while(waitKey(30) != ' '){
        if(gameState == 0){
            if(controllerLeft.confirmVal == 1 && controllerRight.confirmVal == 1){
                gameState=1;
                cvDestroyWindow("treshold 1");
                cvDestroyWindow("treshold 2");
                cvDestroyWindow("Controllers [Player 1]");
                cvDestroyWindow("Controllers [Player 2]");
                continue;
            }
            config();
        }
        else if(gameState == 1){
            playGame();
        }
    }
}
