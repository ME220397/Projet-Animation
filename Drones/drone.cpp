#include "drone.h"

Drone::Drone(QVector3D position, float size, float temps_restant)
{
    this->position = position;
    this->size = size;
    this->temps_restant = temps_restant;
    this->temps_max = temps_restant;
}

void Drone::draw(QMatrix4x4 projection, QMatrix4x4 view, int nLines, int nFaces, bool show_axis){
    // Affiche les arêtes du drone
    vbo_line.bind();
    program_line->bind();
    QMatrix4x4 modelLineMatrix;
    modelLineMatrix.translate(position);

    program_line->setUniformValue("projectionMatrix", projection);
    program_line->setUniformValue("viewMatrix", view);
    program_line->setUniformValue("modelMatrix", modelLineMatrix);

    program_line->setUniformValue("size", size);
    program_line->setAttributeBuffer("in_position", GL_FLOAT ,0, 3,  6*sizeof(GLfloat));
    program_line->setAttributeBuffer("col", GL_FLOAT, 3*sizeof (GLfloat), 3, 6*sizeof(GLfloat));

    program_line->enableAttributeArray("in_position");
    program_line->enableAttributeArray("col");

    glDrawArrays(GL_LINES, 0, nLines);

    program_line->disableAttributeArray("in_position");
    program_line->disableAttributeArray("col");

    program_line->release();

    // Affiche les faces du drone
    vbo_mesh.bind();
    program_mesh->bind();

    QMatrix4x4 modelMatrixmesh;
    modelMatrixmesh.translate(position);


    program_mesh->setUniformValue("projectionMatrix", projection);
    program_mesh->setUniformValue("viewMatrix", view);
    program_mesh->setUniformValue("modelMatrix", modelMatrixmesh);

    program_mesh->setUniformValue("size", size);
    program_mesh->setAttributeBuffer("in_position", GL_FLOAT ,0, 3,  6*sizeof(GLfloat));
    program_mesh->setAttributeBuffer("col", GL_FLOAT, 3*sizeof (GLfloat), 3, 6*sizeof(GLfloat));

    program_mesh->enableAttributeArray("in_position");
    program_mesh->enableAttributeArray("col");

    glDrawArrays(GL_TRIANGLES, 0, nFaces);

    program_mesh->disableAttributeArray("in_position");
    program_mesh->disableAttributeArray("col");

    program_mesh->release();

    // Afficher les allant du drone vers le sol
    if(show_axis){
        vbo_axes.bind();
        program_axe->bind();
        QMatrix4x4 modelAxeMatrix;
        QVector3D position_transi = position;
        position_transi[1] = 0.0;
        modelAxeMatrix.translate(position_transi);

        program_axe->setUniformValue("projectionMatrix", projection);
        program_axe->setUniformValue("viewMatrix", view);
        program_axe->setUniformValue("modelMatrix", modelAxeMatrix);

        program_axe->setUniformValue("size", 1.0f);
        program_axe->setAttributeBuffer("in_position", GL_FLOAT ,0, 3,  6*sizeof(GLfloat));
        program_axe->setAttributeBuffer("col", GL_FLOAT, 3*sizeof (GLfloat), 3, 6*sizeof(GLfloat));

        program_axe->enableAttributeArray("in_position");
        program_axe->enableAttributeArray("col");

        glDrawArrays(GL_LINES, 0, 2);

        program_axe->disableAttributeArray("in_position");
        program_axe->disableAttributeArray("col");

        program_axe->release();
    }
}

void Drone::init(QOpenGLShaderProgram *programM, QOpenGLShaderProgram *programL, QOpenGLBuffer vboM, QOpenGLBuffer vboL){
    program_mesh = programM;
    program_line = programL;
    vbo_line = vboL;
    vbo_mesh = vboM;
}


void Drone::init_axes(QOpenGLShaderProgram *programM, QOpenGLBuffer vboM){
    program_axe = programM;
    vbo_axes = vboM;
}

float Drone::get_temps_ecoule(){
    return temps_max - temps_restant;
}

void Drone::set_vitesse(QVector3D vitesse){
    this->vitesse = vitesse;
}

void Drone::animate(float dt, float i, float framerate){
    if(temps_restant > 0.0f ){
        position = position + (vitesse - position)*i/framerate;
        temps_restant-=dt;
    }
}

void Drone::set_position(QVector3D p){
    position = p;
}

void Drone::set_temps_restant(float t){
    temps_restant = temps_max - t;
}
