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
        void draw(QMatrix4x4 projection, QMatrix4x4 view, int nLines, int nFaces, bool show_axis);
        void init(QOpenGLShaderProgram *programM, QOpenGLShaderProgram *programL, QOpenGLBuffer vboM, QOpenGLBuffer vboL);
        void init_axes(QOpenGLShaderProgram *programM, QOpenGLBuffer vboM);
        QVector3D get_position(){
            return position;
        }
        QVector3D get_vitesse(){
            return vitesse;
        }
    private:
        QVector3D position;
        float size;
        QVector3D vitesse;

        QOpenGLShaderProgram *program_mesh;
        QOpenGLShaderProgram *program_line;
        QOpenGLShaderProgram *program_axe;
        QOpenGLBuffer vbo_mesh;
        QOpenGLBuffer vbo_line;
        QOpenGLBuffer vbo_axes;
};

#endif // DRONE_H
