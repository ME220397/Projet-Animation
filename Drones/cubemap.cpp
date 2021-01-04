#include "cubemap.h"

CubeMap::CubeMap(QOpenGLWidget * parent)
{

    program_bill = new QOpenGLShaderProgram(parent);
    program_bill2 = new QOpenGLShaderProgram(parent);
    program_bill3 = new QOpenGLShaderProgram(parent);
    program_bill4 = new QOpenGLShaderProgram(parent);
    program_bill5 = new QOpenGLShaderProgram(parent);
    program_bill6 = new QOpenGLShaderProgram(parent);

}

void CubeMap::init_shader(){
    // shader de billboard
    program_bill->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/billboard.vsh");
    program_bill->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/billboard.fsh");
    if (! program_bill->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_bill->log();
    }
    program_bill->setUniformValue("texture", 0);

    program_bill2->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/billboard.vsh");
    program_bill2->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/billboard.fsh");
    if (! program_bill2->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_bill2->log();
    }
    program_bill2->setUniformValue("texture", 0);

    program_bill3->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/billboard.vsh");
    program_bill3->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/billboard.fsh");
    if (! program_bill3->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_bill3->log();
    }
    program_bill3->setUniformValue("texture", 0);

    program_bill4->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/billboard.vsh");
    program_bill4->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/billboard.fsh");
    if (! program_bill4->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_bill4->log();
    }
    program_bill4->setUniformValue("texture", 0);

    program_bill5->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/billboard.vsh");
    program_bill5->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/billboard.fsh");
    if (! program_bill5->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_bill5->log();
    }
    program_bill5->setUniformValue("texture", 0);

    program_bill6->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/billboard.vsh");
    program_bill6->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/billboard.fsh");
    if (! program_bill6->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_bill6->log();
    }
    program_bill6->setUniformValue("texture", 0);
}

void CubeMap::loadCubeMap(){

    QVector<GLfloat> vert_data;
    // Premier billboard
    GLfloat vertices_right[] = {
        -0.5f, 0.5f, 0.0f,
        -0.5f,-0.5f, 0.0f,
         0.5f, 0.5f, 00.0f,
        -0.5f,-0.5f, 00.0f,
         0.5f,-0.5f, 00.0f,
         0.5f, 0.5f, 00.0f
    };

    GLfloat colors_right[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };

    for(int i = 0; i<6; i++){
        for(int j=0; j<3; j++){
            vert_data.append(vertices_right[i*3+j]);
        }
        for(int j=0; j<2; j++){
            vert_data.append(colors_right[i*2+j]);
        }
    }

    vbo_bill.create();
    vbo_bill.bind();
    vbo_bill.allocate(vert_data.constData(), vert_data.count() * sizeof(GLfloat));


    QVector<GLfloat> vert_data2;
    // Deuxieme billboard
   GLfloat vertices_left[] = {
       -0.5f, 0.5f, 0.0f,
       -0.5f,-0.5f, 0.0f,
        0.5f, 0.5f, 00.0f,
       -0.5f,-0.5f, 00.0f,
        0.5f,-0.5f, 00.0f,
        0.5f, 0.5f, 00.0f
    };



    GLfloat colors_left[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };

    for(int i = 0; i<6; i++){
        for(int j=0; j<3; j++){
            vert_data2.append(vertices_left[i*3+j]);
        }
        for(int j=0; j<2; j++){
            vert_data2.append(colors_left[i*2+j]);
        }
    }

    vbo2.create();
    vbo2.bind();
    vbo2.allocate(vert_data2.constData(), vert_data2.count() * sizeof (GLfloat));

    GLfloat vertices_front[] = {
       0.0f,0.5f,-0.5f,
       0.0f,-0.5f,-0.5f,
        0.0f,0.5f, 0.5f,
       0.0f,-0.5f,-0.5f,
        0.0f,-0.5f,0.5f,
       0.0f, 0.5f, 0.5f
    };

    GLfloat colors_front[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
    };

    QVector<GLfloat> vert_data3;
    for(int i = 0; i<6; i++){
        for(int j=0; j<3; j++){
            vert_data3.append(vertices_front[i*3+j]);
        }
        for(int j=0; j<2; j++){
            vert_data3.append(colors_front[i*2+j]);
        }
    }

    vbo3.create();
    vbo3.bind();
    vbo3.allocate(vert_data3.constData(), vert_data3.count() * sizeof (GLfloat));

    GLfloat vertices_back[] = {
       0.0f,0.5f,-0.5f,
       0.0f,-0.5f,-0.5f,
        0.0f,0.5f, 0.5f,
       0.0f,-0.5f,-0.5f,
        0.0f,-0.5f,0.5f,
       0.0f, 0.5f, 0.5f
    };

    GLfloat colors_back[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };

    QVector<GLfloat> vert_data4;
    for(int i = 0; i<6; i++){
        for(int j=0; j<3; j++){
            vert_data4.append(vertices_back[i*3+j]);
        }
        for(int j=0; j<2; j++){
            vert_data4.append(colors_back[i*2+j]);
        }
    }

    vbo4.create();
    vbo4.bind();
    vbo4.allocate(vert_data4.constData(), vert_data4.count() * sizeof (GLfloat));

    GLfloat vertices_bot[] = {
      0.5f,0.0f,-0.5f,
       -0.5f,0.0f,-0.5f,
       0.5f, 0.0f, 0.5f,
       -0.5f,0.0f,-0.5f,
       -0.5f, 0.0f,0.5f,
       0.5f, 0.0f, 0.5f
    };

    GLfloat colors_bot[] = {
        0.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 0.0f,
        0.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, 0.0f
    };

    QVector<GLfloat> vert_data5;
    for(int i = 0; i<6; i++){
        for(int j=0; j<3; j++){
            vert_data5.append(vertices_bot[i*3+j]);
        }
        for(int j=0; j<2; j++){
            vert_data5.append(colors_bot[i*2+j]);
        }
    }

    vbo5.create();
    vbo5.bind();
    vbo5.allocate(vert_data5.constData(), vert_data5.count() * sizeof (GLfloat));

    vbo6.create();
    vbo6.bind();
    vbo6.allocate(vert_data5.constData(), vert_data5.count() * sizeof (GLfloat));





    QImage image_particule("./skybox/right.jpg");
    if (image_particule.isNull())
        qDebug() << "load image puff.png failed";
    textures[0] = new QOpenGLTexture(image_particule);

    QImage image_particule2("./skybox/left.jpg");
    if (image_particule2.isNull())
        qDebug() << "load image puff.png failed";
    textures[1] = new QOpenGLTexture(image_particule2);

    QImage image_particule3("./skybox/front.jpg");
    if (image_particule3.isNull())
        qDebug() << "load image puff.png failed";
    textures[2] = new QOpenGLTexture(image_particule3);

    QImage image_particule4("./skybox/back.jpg");
    if (image_particule4.isNull())
        qDebug() << "load image puff.png failed";
    textures[3] = new QOpenGLTexture(image_particule4);

    QImage image_particule5("./skybox/bottom.jpg");
    if (image_particule5.isNull())
        qDebug() << "load image puff.png failed";
    textures[4] = new QOpenGLTexture(image_particule5);

    QImage image_particule6("./skybox/top.jpg");
    if (image_particule6.isNull())
        qDebug() << "load image puff.png failed";
    textures[5] = new QOpenGLTexture(image_particule6);
}

void CubeMap::draw(QMatrix4x4 projectionMatrix, QMatrix4x4 viewMatrix){
    // Affichage d'une particule
        vbo_bill.bind();
        program_bill->bind(); // active le shader program des bills

        // billboard 1
        QMatrix4x4 modelMatrixbill;
        modelMatrixbill.translate(0.0f, 50.0f, 300.0f);
        program_bill->setUniformValue("projectionMatrix", projectionMatrix);
        program_bill->setUniformValue("viewMatrix", viewMatrix);
        program_bill->setUniformValue("modelMatrix", modelMatrixbill);
        program_bill->setUniformValue("particleSize", 1000.0f);

        program_bill->setAttributeBuffer("in_position", GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
        program_bill->setAttributeBuffer("in_uv", GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
        program_bill->enableAttributeArray("in_position");
        program_bill->enableAttributeArray("in_uv");

        textures[0]->bind();
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisable(GL_BLEND);
        textures[0]->release();

        program_bill->disableAttributeArray("in_position");
        program_bill->disableAttributeArray("in_uv");
        program_bill->release();

        vbo2.bind();
        program_bill2->bind();
        QMatrix4x4 modelMatrixbill2;
        modelMatrixbill2.translate(0.0f, 50.0f, -300.0f);

        program_bill2->setUniformValue("projectionMatrix", projectionMatrix);
        program_bill2->setUniformValue("viewMatrix", viewMatrix);
        program_bill2->setUniformValue("modelMatrix", modelMatrixbill2);
        program_bill2->setUniformValue("particleSize", 1000.0f);

        program_bill2->setAttributeBuffer("in_position", GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
        program_bill2->setAttributeBuffer("in_uv", GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
        program_bill2->enableAttributeArray("in_position");
        program_bill2->enableAttributeArray("in_uv");

        textures[1]->bind();
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisable(GL_BLEND);
        textures[1]->release();

        program_bill2->disableAttributeArray("in_position");
        program_bill2->disableAttributeArray("in_uv");
        program_bill2->release();

        vbo3.bind();
        program_bill3->bind();
        QMatrix4x4 modelMatrixbill3;
        modelMatrixbill3.translate(300.0f, 50.0f, 0.0f);

        program_bill3->setUniformValue("projectionMatrix", projectionMatrix);
        program_bill3->setUniformValue("viewMatrix", viewMatrix);
        program_bill3->setUniformValue("modelMatrix", modelMatrixbill3);
        program_bill3->setUniformValue("particleSize", 1000.0f);

        program_bill3->setAttributeBuffer("in_position", GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
        program_bill3->setAttributeBuffer("in_uv", GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
        program_bill3->enableAttributeArray("in_position");
        program_bill3->enableAttributeArray("in_uv");

        textures[2]->bind();
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisable(GL_BLEND);
        textures[2]->release();

        program_bill3->disableAttributeArray("in_position");
        program_bill3->disableAttributeArray("in_uv");
        program_bill3->release();

        /*vbo4.bind();
        program_bill4->bind();
        QMatrix4x4 modelMatrixbill4;
        modelMatrixbill4.translate(-300.0f, 50.0f, 0.0f);

        program_bill4->setUniformValue("projectionMatrix", projectionMatrix);
        program_bill4->setUniformValue("viewMatrix", viewMatrix);
        program_bill4->setUniformValue("modelMatrix", modelMatrixbill4);
        program_bill4->setUniformValue("particleSize", 1000.0f);

        program_bill4->setAttributeBuffer("in_position", GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
        program_bill4->setAttributeBuffer("in_uv", GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
        program_bill4->enableAttributeArray("in_position");
        program_bill4->enableAttributeArray("in_uv");

        textures[3]->bind();
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisable(GL_BLEND);
        textures[3]->release();

        program_bill4->disableAttributeArray("in_position");
        program_bill4->disableAttributeArray("in_uv");
        program_bill4->release();*/

        /*vbo5.bind();
        program_bill5->bind();
        QMatrix4x4 modelMatrixbill5;
        modelMatrixbill5.translate(0.0f, 300.0f, 0.0f);

        program_bill5->setUniformValue("projectionMatrix", projectionMatrix);
        program_bill5->setUniformValue("viewMatrix", viewMatrix);
        program_bill5->setUniformValue("modelMatrix", modelMatrixbill5);
        program_bill5->setUniformValue("particleSize", 1000.0f);

        program_bill5->setAttributeBuffer("in_position", GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
        program_bill5->setAttributeBuffer("in_uv", GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
        program_bill5->enableAttributeArray("in_position");
        program_bill5->enableAttributeArray("in_uv");

        textures[4]->bind();
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisable(GL_BLEND);
        textures[4]->release();

        program_bill5->disableAttributeArray("in_position");
        program_bill5->disableAttributeArray("in_uv");
        program_bill5->release();

        vbo6.bind();
        program_bill6->bind();
        QMatrix4x4 modelMatrixbill6;
        modelMatrixbill6.translate(0.0f, -300.0f, 0.0f);

        program_bill6->setUniformValue("projectionMatrix", projectionMatrix);
        program_bill6->setUniformValue("viewMatrix", viewMatrix);
        program_bill6->setUniformValue("modelMatrix", modelMatrixbill6);
        program_bill6->setUniformValue("particleSize", 1000.0f);

        program_bill6->setAttributeBuffer("in_position", GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
        program_bill6->setAttributeBuffer("in_uv", GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
        program_bill6->enableAttributeArray("in_position");
        program_bill6->enableAttributeArray("in_uv");

        textures[5]->bind();
        glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
        glEnable(GL_BLEND);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glDisable(GL_BLEND);
        textures[5]->release();

        program_bill6->disableAttributeArray("in_position");
        program_bill6->disableAttributeArray("in_uv");
        program_bill6->release();*/

}

void CubeMap::destroy_vbo(){
    vbo_bill.destroy();
    vbo2.destroy();
    vbo3.destroy();
    vbo4.destroy();
    vbo5.destroy();
    vbo6.destroy();

}
