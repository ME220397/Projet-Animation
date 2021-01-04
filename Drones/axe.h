#ifndef AXE_H
#define AXE_H

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>

class Axe
{ 
public:
    Axe(QOpenGLWidget * parent);
    void load_repere();
    void draw_repere(QMatrix4x4 projection, QMatrix4x4 view);
    void init_shaders();
private:
    QOpenGLBuffer vbo_repere;
    QOpenGLShaderProgram *program_repere; // Pour le repere
};

#endif // AXE_H
