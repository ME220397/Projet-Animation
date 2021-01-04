#include "cubemap.h"

CubeMap::CubeMap()
{
    // On charge les partie de la skyBox
    faces.push_back("right.jpg");
    faces.push_back("left.jpg");
    faces.push_back("top.jpg");
    faces.push_back("bottom.jpg");
    faces.push_back("front.jpg");
    faces.push_back("back.jpg");

}

void CubeMap::loadCubeMap(){

}
