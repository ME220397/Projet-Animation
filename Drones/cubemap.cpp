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
    // Premier billboard
    GLfloat vertices_right[] = {
       1.0f, -1.0f, -1.0f,
       1.0f,-1.0f, 1.0f,
       1.0f, 1.0f, 1.0f,

       1.0f,-1.0f, -1.0f,
       1.0f, 1.0f, 1.0f,
       1.0f, -1.0f, 1.0f
    };

    // Deuxieme billboard
    GLfloat vertices_left[] = {
       -1.0f, -1.0f, 1.0f,
       -1.0f,-1.0f, -1.0f,
       -1.0f, 1.0f, -1.0f,

       -1.0f,-1.0f, 1.0f,
       -1.0f, 1.0f, -1.0f,
       -1.0f, -1.0f, -1.0f
    };

    GLfloat vertices_front[] = {
        -1.0f, -1.0f, 1.0f,
        1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,

        -1.0f,-1.0f, 1.0f,
        1.0f, 1.0f, 1.0f,
        1.0f, -1.0f, 1.0f
    };

    GLfloat vertices_back[] = {
        -1.0f, -1.0f, -1.0f,
        1.0f,-1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,

        -1.0f,-1.0f, -1.0f,
        1.0f, 1.0f, -1.0f,
        1.0f, -1.0f, -1.0f
    };

    GLfloat vertices_bot[] = {
        1.0f, -1.0f, 1.0f,
        1.0f, -1.0f,
    };
}
