#ifndef CUBEMAP_H
#define CUBEMAP_H

#include<iostream>

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLTexture>
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
    CubeMap(QOpenGLWidget * parent);
    void init_shader();
    void loadCubeMap();
    void draw(QMatrix4x4 proj, QMatrix4x4 view);
    void destroy_vbo();
private:
    vector<std::string> faces;
    QOpenGLTexture *textures[6];
    QOpenGLBuffer vbo_bill;
    QOpenGLBuffer vbo2;
    QOpenGLBuffer vbo3;
    QOpenGLBuffer vbo4;
    QOpenGLBuffer vbo5;
    QOpenGLBuffer vbo6;
    QOpenGLShaderProgram *program_bill;
    QOpenGLShaderProgram *program_bill2;
    QOpenGLShaderProgram *program_bill3;
    QOpenGLShaderProgram *program_bill4;
    QOpenGLShaderProgram *program_bill5;
    QOpenGLShaderProgram *program_bill6;
};

#endif // CUBEMAP_H
