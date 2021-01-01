#ifndef DRONE_H
#define DRONE_H
#include<QVector3D>
#include<iostream>

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

class Drone
{

    public:
        Drone(QVector3D position, float size);
        void draw(QMatrix4x4 projection, QMatrix4x4 view, int nLines, int nFaces);
        void init(QOpenGLShaderProgram *programM, QOpenGLShaderProgram *programL, QOpenGLBuffer vboM, QOpenGLBuffer vboL);
    private:
        QVector3D position;
        float size;

        QOpenGLShaderProgram *program_mesh;
        QOpenGLShaderProgram *program_line;
        QOpenGLBuffer vbo_mesh;
        QOpenGLBuffer vbo_line;
};

#endif // DRONE_H
