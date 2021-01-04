#include "dronefactory.h"

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


static const char *vertexShaderSourceM =
    "attribute vec4 in_position;               \n"
    "attribute lowp vec4 col;               \n"
    "varying lowp vec4 color;               \n"
    "uniform mat4 projectionMatrix;         \n"
    "uniform mat4 viewMatrix;               \n"
    "uniform mat4 modelMatrix;              \n"
    "uniform float size;                    \n"
    "void main() {                          \n"
    "   color = col;                        \n"
    "   vec4 pos = vec4(vec3(in_position) * size, 1.0);                   \n"
    "   gl_Position = projectionMatrix * viewMatrix * modelMatrix * pos;          \n"
    "}                                      \n";

// shader appliqué à un fragment = un morceau rasterisé d'une primitive graphique
static const char *fragmentShaderSourceM =
    "varying lowp vec4 color;               \n"
    "void main() {                          \n"
    "   gl_FragColor = color;               \n"
    "}                                      \n";


DroneFactory::DroneFactory(QOpenGLWidget * parent)
{
    program_mesh = new QOpenGLShaderProgram(parent);
    program_line = new QOpenGLShaderProgram(parent);
    program_trajectory = new QOpenGLShaderProgram(parent);
    program_axe = new QOpenGLShaderProgram(parent);
    // fenêtre de sélection des fichiers
    QString fileName = "./obj/drone.obj";

    // chargement du fichier .obj dans la variable globale "mesh"
    OpenMesh::IO::read_mesh(mesh, fileName.toUtf8().constData());
    resetAllColorsAndThickness(&mesh);

    // Récupération des données du Json
    reader = new JsonReader("./json/waypoints.json");
    nb_drones = reader->get_nb_drones();
    // On s'assure qu'il y a bien 20 drones comme dans le json
    assert(nb_drones == 20);

    QVector3D pos = reader->accessPosition(0, 0);
    d = QVector3D(0,400,0) - pos;

    // On récupère les positions du json translaté et divé par 40 pour chaque dornes
    for(int i = 0; i<nb_drones; i++){
        trajectories.push_back(reader->compute_trajectory(i, d));
    }
    assert(!trajectories.empty());

    // On récupère le nombre de ligne nécessaire pour affiché les trajectoires
    n_lines_trajectory = 0;
    for(std::vector<QVector3D> positions : trajectories){
        int n = positions.size();
        n_lines_trajectory += (n-1)*2;
    }
}

void DroneFactory::create_drones(){
    //Création des drones
    for(int i = 0; i<nb_drones; i++){
        add_drone(i);
    }
}

void DroneFactory::resetAllColorsAndThickness(MyMesh* _mesh)
{
    for (MyMesh::VertexIter curVert = _mesh->vertices_begin(); curVert != _mesh->vertices_end(); curVert++)
    {
        _mesh->data(*curVert).thickness = 1;
        _mesh->set_color(*curVert, MyMesh::Color(0, 0, 0));
    }

    for (MyMesh::FaceIter curFace = _mesh->faces_begin(); curFace != _mesh->faces_end(); curFace++)
    {
        _mesh->set_color(*curFace, MyMesh::Color(150, 150, 150));
    }

    for (MyMesh::EdgeIter curEdge = _mesh->edges_begin(); curEdge != _mesh->edges_end(); curEdge++)
    {
        _mesh->data(*curEdge).thickness = 1;
        _mesh->set_color(*curEdge, MyMesh::Color(0, 0, 0));
    }
}

void DroneFactory::loadMesh(){
    MyMesh *_mesh = &mesh;
    n_faces = _mesh->n_faces();
    n_edges = _mesh->n_edges();

    // On charge les points des faces d'un drone
    GLfloat vertices[n_faces*3*3];
    GLfloat colors[n_faces*3*3];
    int id = 0;
    for(MyMesh::FaceIter f_it = _mesh->faces_begin(); f_it != _mesh->faces_end(); f_it++){
        HalfedgeHandle eh = _mesh->halfedge_handle(*f_it);

        MyMesh::VertexHandle v0, v1, v2;
        v0 = _mesh->from_vertex_handle(eh);
        v1 = _mesh->to_vertex_handle(eh);
        eh = _mesh->next_halfedge_handle(eh);
        v2 = _mesh->to_vertex_handle(eh);

        MyMesh::Point a,b,c;
        a = _mesh->point(v0);
        b = _mesh->point(v1);
        c = _mesh->point(v2);

        vertices[id] = float(a[0]); vertices[id+1] = float(a[1]); vertices[id+2] = float(a[2]);
        vertices[id+3] = float(b[0]); vertices[id+4] = float(b[1]); vertices[id+5] = float(b[2]);
        vertices[id+6] = float(c[0]); vertices[id+7] = float(c[1]); vertices[id+8] = float(c[2]);

        colors[id] = 0.5; colors[id+1] = 0.5; colors[id+2] = 0.5;
        colors[id+3] = 0.5; colors[id+4] = 0.5; colors[id+5] = 0.5;
        colors[id+6] = 0.5; colors[id+7] = 0.5; colors[id+8] = 0.5;

        id+=9;
    }
    QVector<GLfloat> vert_data_mesh;
    for(int i = 0; i<n_faces*3; i++){
        for(int j = 0; j<3; j++){
            vert_data_mesh.append(vertices[i*3+j]);
        }
        for(int j = 0; j<3; j++){
            vert_data_mesh.append(colors[i*3+j]);
        }
    }
    vbo_mesh.create();
    vbo_mesh.bind();
    vbo_mesh.allocate(vert_data_mesh.constData(), vert_data_mesh.count() * int(sizeof (GLfloat)));

    // On charge les arêtes du drones

    GLfloat vert_line[n_edges*2*3];
    GLfloat col_line[n_edges*2*3];
    id = 0;

    for(MyMesh::EdgeIter e_it = _mesh->edges_begin(); e_it != _mesh->edges_end(); e_it++){
        HalfedgeHandle eh = _mesh->halfedge_handle(*e_it, 0);

        VertexHandle v0, v1;
        v0 = _mesh->from_vertex_handle(eh);
        v1 = _mesh->to_vertex_handle(eh);

        MyMesh::Point a, b;
        a = _mesh->point(v0);
        b = _mesh->point(v1);
        vert_line[id] = a[0];vert_line[id+1] = a[1];vert_line[id+2] = a[2];
        vert_line[id+3] = b[0];vert_line[id+4] = b[1];vert_line[id+5] = b[2];

        col_line[id] = 0;col_line[id+1] = 0;col_line[id+2] = 0;
        col_line[id+3] = 0;col_line[id+4] = 0;col_line[id+5] = 0;

        id+=6;
    }

    QVector<GLfloat> vert_data_line;
    for(int i = 0; i<n_edges*2; i++){
        for(int j = 0; j<3; j++){
            vert_data_line.append(vert_line[i*3+j]);
        }
        for(int j = 0; j<3; j++){
            vert_data_line.append(col_line[i*3+j]);
        }
    }

    vbo_line.create();
    vbo_line.bind();
    vbo_line.allocate(vert_data_line.constData(), vert_data_line.count() * int(sizeof(GLfloat)));

    //On charge les lignes des trajectoire

    QVector<GLfloat> vert_traject;
    for(std::vector<QVector3D> positions : trajectories){
        int n = positions.size();
        for(int i = 0; i<(n-1) ; i++){
            QVector3D p = positions.at(i);
            QVector3D q = positions.at(i+1);

            vert_traject.append((float)p[0]);
            vert_traject.append((float)p[1]);
            vert_traject.append((float)p[2]);

            vert_traject.append(0.0f);
            vert_traject.append(1.0f);
            vert_traject.append(0.0f);

            vert_traject.append((float)q[0]);
            vert_traject.append((float)q[1]);
            vert_traject.append((float)q[2]);

            vert_traject.append(0.0f);
            vert_traject.append(1.0f);
            vert_traject.append(0.0f);
        }
    }
    //On charge les couleurs des trajectoires
    /*for(std::vector<QVector3D> positions : trajectories){
        int n = positions.size();
        for(int i = 0; i<(n-1) ; i++){

            vert_traject.append(0.0f);
            vert_traject.append(0.0f);
            vert_traject.append(0.0f);

            vert_traject.append(0.0f);
            vert_traject.append(0.0f);
            vert_traject.append(0.0f);
        }
    }*/

    vbo_trajectory.create();
    vbo_trajectory.bind();
    vbo_trajectory.allocate(vert_traject.constData(), vert_traject.count() * sizeof(GLfloat));

    //Chargement des axes
    QVector3D position = reader->accessPosition(0,0);
    position+=d;
    position/=40;
    float a, b, c;//position du drone
    a = position[0];
    b = position[1];
    c = position[2];

    float x, y, z;//position du point d'intersection entre le plan et la droite partant du drone
    x = position[0];
    y = 0.0;
    z = position[2];

    /*GLfloat vert_axes[6] ={
        a, b, c,
        x, y, z
    };*/
    QVector<GLfloat> vert_data_axes;
    vert_data_axes.append(a); vert_data_axes.append(b); vert_data_axes.append(c);
    vert_data_axes.append(1.0); vert_data_axes.append(1.0f); vert_data_axes.append(0.0f); //couleur a,b,c
    vert_data_axes.append(x); vert_data_axes.append(y); vert_data_axes.append(z);
    vert_data_axes.append(1.0f); vert_data_axes.append(1.0f); vert_data_axes.append(0.0f); //couleur x,y,z

    vbo_axes.create();
    vbo_axes.bind();
    vbo_axes.allocate(vert_data_axes.constData(), vert_data_axes.count()*sizeof (GLfloat));
}

void DroneFactory::delete_vbos(){
    vbo_line.destroy();
    vbo_mesh.destroy();
    vbo_trajectory.destroy();
}

void DroneFactory::init_shaders(){
    program_mesh->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSourceM);
    program_mesh->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSourceM);
    if (! program_mesh->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program (Mesh):");
        qWarning() << program_mesh->log();
    }

    program_line->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    program_line->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    if (! program_line->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program (Line):");
        qWarning() << program_line->log();
    }

    program_trajectory->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    program_trajectory->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    if(! program_trajectory->link()){
        qWarning("Failed to compile and link shader program (Trajectory):");
        qWarning() << program_trajectory->log();
    }

    program_axe->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    program_axe->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    if (! program_axe->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program (Axe):");
        qWarning() << program_axe->log();
    }
}

void DroneFactory::add_drone(int id_drone){
    QVector3D position_start = reader->accessPosition(id_drone, 0);
    position_start +=d;
    Drone d(position_start/40, 2.);
    d.init(program_mesh, program_line, vbo_mesh, vbo_line);
    d.init_axes(program_axe, vbo_axes);
    drones.push_back(d);
}

void DroneFactory::draw(QMatrix4x4 projection, QMatrix4x4 view, bool show_axis){
    //vbo_line.bind();
    //vbo_mesh.bind();
    for(Drone d: drones){
        //d.init(program_mesh, program_line, vbo_mesh, vbo_line);
        d.draw(projection, view, n_edges*2, n_faces*3, show_axis);
    }
}

void DroneFactory::draw_trajectories(QMatrix4x4 projection, QMatrix4x4 view){
    vbo_trajectory.bind();
    program_trajectory->bind();
    QMatrix4x4 modelLineMatrix;
    modelLineMatrix.translate(0.0, 0.0, 0.0);

    program_trajectory->setUniformValue("projectionMatrix", projection);
    program_trajectory->setUniformValue("viewMatrix", view);
    program_trajectory->setUniformValue("modelMatrix", modelLineMatrix);

    program_trajectory->setUniformValue("size", 1.f);
    program_trajectory->setAttributeBuffer("in_position", GL_FLOAT ,0, 3,  6*sizeof(GLfloat));
    program_trajectory->setAttributeBuffer("col", GL_FLOAT, 3*sizeof (GLfloat), 3, 6*sizeof(GLfloat));

    program_trajectory->enableAttributeArray("in_position");
    program_trajectory->enableAttributeArray("col");

    glDrawArrays(GL_LINES, 0, n_lines_trajectory);

    program_trajectory->disableAttributeArray("in_position");
    program_trajectory->disableAttributeArray("col");

    program_trajectory->release();
}
