#include <cstdio>

#include "GameManager.hpp"

using namespace std;
using namespace cv;

int main(int argc, char * argv[]){
    GameManager game;
    if(!game.openCam()){
        return 0;
    }
    game.play();
    return 0;
}
