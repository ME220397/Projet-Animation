// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "glarea.h"
#include <QDebug>
#include <QSurfaceFormat>
#include <QMatrix4x4>


GLArea::GLArea(QWidget *parent) :
    QOpenGLWidget(parent)
{
    QSurfaceFormat sf;
    sf.setDepthBufferSize(24);
    sf.setSamples(16);
    setFormat(sf);

    setEnabled(true);                   // événements clavier et souris
    setFocusPolicy(Qt::StrongFocus);    // accepte focus
    setFocus();                         // donne le focus

    timer = new QTimer(this);
    timer->setInterval(50);           // msec
    connect (timer, SIGNAL(timeout()), this, SLOT(onTimeout()));
    timer->start();
    elapsedTimer.start();

    test = new DroneFactory(this);
    currentFrame = 0;
    maxFrame = test->get_max_frame();
    repere = new Axe(this);
}


GLArea::~GLArea()
{
    delete timer;

    // Contrairement aux méthodes virtuelles initializeGL, resizeGL et repaintGL,
    // dans le destructeur le contexte GL n'est pas automatiquement rendu courant.
    makeCurrent();
    tearGLObjects();
    doneCurrent();
}


void GLArea::initializeGL()
{
    initializeOpenGLFunctions();
    glClearColor(0.1f,0.4f,1.0f,1.0f);
    glEnable(GL_DEPTH_TEST);

    makeGLObjects();

    // shader du sol
    program_sol = new QOpenGLShaderProgram(this);
    program_sol->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/simple.vsh");
    program_sol->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/simple.fsh");
    if (! program_sol->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_sol->log();
    }
    program_sol->setUniformValue("texture", 0);

    // shader de billboard
    program_particule = new QOpenGLShaderProgram(this);
    program_particule->addShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/billboard.vsh");
    program_particule->addShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/billboard.fsh");
    if (! program_particule->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_particule->log();
    }
    program_particule->setUniformValue("texture", 0);

    repere->init_shaders();
    test->init_shaders();

}


void GLArea::makeGLObjects()
{
    // Création du sol
    float tailleSol = 20.0f;

    GLfloat vertices_sol[] = {
       -tailleSol, 0.0f,-tailleSol,
       -tailleSol, 0.0f, tailleSol,
        tailleSol, 0.0f, tailleSol,
        tailleSol, 0.0f, tailleSol,
        tailleSol, 0.0f,-tailleSol,
       -tailleSol, 0.0f,-tailleSol
    };

    GLfloat texCoords_sol[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 0.0f
        };

    QVector<GLfloat> vertData_sol;
    for (int i = 0; i < 6; ++i) {
        // coordonnées sommets
        for (int j = 0; j < 3; j++)
            vertData_sol.append(vertices_sol[i*3+j]*30.0f);
        // coordonnées texture
        for (int j = 0; j < 2; j++)
            vertData_sol.append(texCoords_sol[i*2+j]);
    }

    vbo_sol.create();
    vbo_sol.bind();
    vbo_sol.allocate(vertData_sol.constData(), vertData_sol.count() * int(sizeof(GLfloat)));


    // Création d'une particule de fumée
    GLfloat vertices_particule[] = {
       -0.5f, 0.5f, 0.0f,
       -0.5f,-0.5f, 0.0f,
        0.5f, 0.5f, 0.0f,
       -0.5f,-0.5f, 0.0f,
        0.5f,-0.5f, 0.0f,
        0.5f, 0.5f, 0.0f
    };

    GLfloat texCoords_particule[] = {
            0.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 0.0f,
            0.0f, 1.0f,
            1.0f, 1.0f,
            1.0f, 0.0f
        };

    QVector<GLfloat> vertData_particule;
    for (int i = 0; i < 6; ++i) {
        // coordonnées sommets
        for (int j = 0; j < 3; j++)
            vertData_particule.append(vertices_particule[i*3+j]);
        // coordonnées texture
        for (int j = 0; j < 2; j++)
            vertData_particule.append(texCoords_particule[i*2+j]);
    }

    vbo_particule.create();
    vbo_particule.bind();
    vbo_particule.allocate(vertData_particule.constData(), vertData_particule.count() * int(sizeof(GLfloat)));


    // Création de textures
    QImage image_sol(":/textures/ground.jpg");
    if (image_sol.isNull())
        qDebug() << "load image ground.jpg failed";
    textures[0] = new QOpenGLTexture(image_sol);

    QImage image_particule(":/textures/puff.png");
    if (image_particule.isNull())
        qDebug() << "load image puff.png failed";
    textures[1] = new QOpenGLTexture(image_particule);

    repere->load_repere();

    test->loadMesh();
    test->create_drones();
}


void GLArea::tearGLObjects()
{
    vbo_sol.destroy();
    vbo_particule.destroy();
    test->delete_vbos();
    repere->destroy_vbo();
    for (int i = 0; i < 2; i++)
        delete textures[i];
}


void GLArea::resizeGL(int w, int h)
{
    glViewport(0, 0, w, h);
    windowRatio = float(w) / h;
}


void GLArea::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Matrice de projection
    QMatrix4x4 projectionMatrix;
    projectionMatrix.perspective(45.0f, windowRatio, 1.0f, 500.0f);

    // Matrice de vue (caméra)
    QMatrix4x4 viewMatrix;
    viewMatrix.translate(xPos, yPos, zPos);
    viewMatrix.rotate(xRot, 1, 0, 0);
    viewMatrix.rotate(yRot, 0, 1, 0);
    viewMatrix.rotate(zRot, 0, 0, 1);

    // Affichage du sol
    if(show_sol){
        vbo_sol.bind();
        program_sol->bind(); // active le shader program du sol

        QMatrix4x4 modelMatrixSol;
        modelMatrixSol.translate(0.0f, 0.0f, 0.0f);
        program_sol->setUniformValue("projectionMatrix", projectionMatrix);
        program_sol->setUniformValue("viewMatrix", viewMatrix);
        program_sol->setUniformValue("modelMatrix", modelMatrixSol);

        program_sol->setAttributeBuffer("in_position", GL_FLOAT, 0, 3, 5 * sizeof(GLfloat));
        program_sol->setAttributeBuffer("in_uv", GL_FLOAT, 3 * sizeof(GLfloat), 2, 5 * sizeof(GLfloat));
        program_sol->enableAttributeArray("in_position");
        program_sol->enableAttributeArray("in_uv");

        textures[0]->bind();
        glDrawArrays(GL_TRIANGLES, 0, 6);
        textures[0]->release();

        program_sol->disableAttributeArray("in_position");
        program_sol->disableAttributeArray("in_uv");
        program_sol->release();
    }

    // Affichage du repere
    if(show_rep)
        repere->draw_repere(projectionMatrix, viewMatrix);

    if(play){
        // Animation des drones
        test->animate(dt);
        currentFrame = test->get_current_frame();
        setFrame(currentFrame);
    }

    // Affichage des drone
    test->draw(projectionMatrix, viewMatrix, show_axis);
    if(show_traject)
        test->draw_trajectories(projectionMatrix, viewMatrix);

}


void GLArea::keyPressEvent(QKeyEvent *ev)
{
    float da = 1.0f;

    switch(ev->key()) {
        case Qt::Key_A :
            xRot -= da;
            update();
            break;

        case Qt::Key_Q :
            xRot += da;
            update();
            break;

        case Qt::Key_Z :
            yRot -= da;
            update();
            break;

        case Qt::Key_S :
            yRot += da;
            update();
            break;

        case Qt::Key_E :
            zRot -= da;
            update();
            break;

        case Qt::Key_D :
            zRot += da;
            update();
            break;
    }
}


void GLArea::keyReleaseEvent(QKeyEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->text();
}


void GLArea::mousePressEvent(QMouseEvent *ev)
{
    lastPos = ev->pos();
}


void GLArea::mouseReleaseEvent(QMouseEvent *ev)
{
    qDebug() << __FUNCTION__ << ev->x() << ev->y() << ev->button();
}


void GLArea::mouseMoveEvent(QMouseEvent *ev)
{
    int dx = ev->x() - lastPos.x();
    int dy = ev->y() - lastPos.y();

    if (ev->buttons() & Qt::LeftButton) {
        xRot += dy;
        yRot += dx;
        update();
    } else if (ev->buttons() & Qt::RightButton) {
        xPos += dx/10.0f;
        yPos -= dy/10.0f;
        update();
    } else if (ev->buttons() & Qt::MidButton) {
        xPos += dx/10.0f;
        zPos += dy;
        update();
    }

    lastPos = ev->pos();
}


void GLArea::onTimeout()
{
    static qint64 old_chrono = elapsedTimer.elapsed(); // static : initialisation la première fois et conserve la dernière valeur
    qint64 chrono = elapsedTimer.elapsed();
    dt = (chrono - old_chrono) / 1000.0f;
    old_chrono = chrono;

    update();
}


void GLArea::on_push_trajectory(){
    if(show_traject){
        show_traject = false;
    }
    else
        show_traject = true;
}

void GLArea::on_push_axis(){
    if(show_axis)
        show_axis = false;
    else show_axis = true;
}

void GLArea::on_push_repere(){
    if(show_rep)
        show_rep = false;
    else show_rep = true;
}

void GLArea::on_push_sol(){
    if(show_sol)
        show_sol = false;
    else show_sol = true;
}

void GLArea::on_push_play(){
    play = true;
}

void GLArea::on_push_pause(){
    play = false;
}

void GLArea::frameChanged(int v){
    if(v >= maxFrame-1){
        play = false;
        qDebug() << "Animation mise en pause. frame = " << v;
    }
    test->set_frame(v);
}

int GLArea::get_max_frame(){
    return maxFrame;
}

int GLArea::get_frame(){
    return currentFrame;
}
