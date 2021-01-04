#include "axe.h"

static const char *vertexShaderSource =
    "attribute vec4 in_position;               \n"
    "attribute lowp vec4 col;               \n"
    "varying lowp vec4 color;               \n"
    "uniform mat4 projectionMatrix;         \n"
    "uniform mat4 viewMatrix;               \n"
    "uniform mat4 modelMatrix;              \n"
    "uniform float size;                    \n"
    "void main() {                          \n"
    "   color = col;                        \n"
    "   vec4 pos = vec4(vec3(in_position.x, in_position.y, in_position.z)*size , 1.0);                   \n"
    "   gl_Position = projectionMatrix * viewMatrix * modelMatrix * pos;          \n"
    "}                                      \n";

// shader appliqué à un fragment = un morceau rasterisé d'une primitive graphique
static const char *fragmentShaderSource =
    "varying lowp vec4 color;               \n"
    "void main() {                          \n"
    "   gl_FragColor = color;               \n"
    "}                                      \n";


Axe::Axe(QOpenGLWidget * parent)
{
    program_repere = new QOpenGLShaderProgram(parent);
}
void Axe::load_repere(){
    QVector<GLfloat> vert_data_repere;
    vert_data_repere.push_back(-0.5f); vert_data_repere.push_back(0.0f); vert_data_repere.push_back(0.0f); // axe X
    vert_data_repere.push_back(0.5f); vert_data_repere.push_back(0.0f); vert_data_repere.push_back(0.0f);

    vert_data_repere.push_back(0.0f); vert_data_repere.push_back(-0.5f); vert_data_repere.push_back(0.0f); // axe Y
    vert_data_repere.push_back(0.0f); vert_data_repere.push_back(0.5f); vert_data_repere.push_back(0.0f);

    vert_data_repere.push_back(0.0f); vert_data_repere.push_back(0.0f); vert_data_repere.push_back(-0.5f); // axe Z
    vert_data_repere.push_back(0.0f); vert_data_repere.push_back(0.0f); vert_data_repere.push_back(0.5f);


    vert_data_repere.push_back(0.0f); vert_data_repere.push_back(0.0f); vert_data_repere.push_back(1.0f); // Couleur axe X en bleu
    vert_data_repere.push_back(0.0f); vert_data_repere.push_back(0.0f); vert_data_repere.push_back(1.0f);

    vert_data_repere.push_back(1.0f); vert_data_repere.push_back(0.0f); vert_data_repere.push_back(0.0f); // Couleur axe Y en rouge
    vert_data_repere.push_back(1.0f); vert_data_repere.push_back(0.0f); vert_data_repere.push_back(0.0f);

    vert_data_repere.push_back(0.0f); vert_data_repere.push_back(1.0f); vert_data_repere.push_back(0.0f); // Couleur axe Z en vert
    vert_data_repere.push_back(0.0f); vert_data_repere.push_back(1.0f); vert_data_repere.push_back(0.0f);

    vbo_repere.create();
    vbo_repere.bind();
    vbo_repere.allocate(vert_data_repere.constData(), vert_data_repere.count()*sizeof (GLfloat));

}

void Axe::draw_repere(QMatrix4x4 projection, QMatrix4x4 view){

    vbo_repere.bind();
    program_repere->bind();
    QMatrix4x4 modelRepereMatrix;
    QVector3D position(0.0,0.0,0.0);
    modelRepereMatrix.translate(position);

    program_repere->setUniformValue("projectionMatrix", projection);
    program_repere->setUniformValue("viewMatrix", view);
    program_repere->setUniformValue("modelMatrix", modelRepereMatrix);

    program_repere->setUniformValue("size", 1.0f);
    program_repere->setAttributeBuffer("in_position", GL_FLOAT ,0, 3,  6*sizeof(GLfloat));
    program_repere->setAttributeBuffer("col", GL_FLOAT, 3*sizeof (GLfloat), 3, 6*sizeof(GLfloat));

    program_repere->enableAttributeArray("in_position");
    program_repere->enableAttributeArray("col");

    glDrawArrays(GL_LINES, 0, 6);

    program_repere->disableAttributeArray("in_position");
    program_repere->disableAttributeArray("col");

    program_repere->release();
}

void Axe::init_shaders(){
    program_repere->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    program_repere->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    if (! program_repere->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program (Repere):");
        qWarning() << program_repere->log();
    }
}
