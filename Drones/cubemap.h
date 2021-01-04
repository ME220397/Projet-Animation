#ifndef CUBEMAP_H
#define CUBEMAP_H

#include<iostream>

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
    #include <gl.h>
    #include <glu.h>
    #include <glext.h>
#else
    #include <GL/gl.h>
    #include <GL/glu.h>
    #include <GL/glx.h>
    #include <GL/glext.h>
    #include <GL/glut.h>
#endif

using namespace std;
class CubeMap
{
public:
    CubeMap();
private:

    void loadCubeMap();

    vector<std::string> faces;
};

#endif // CUBEMAP_H
