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

    // Load faces points in a list
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
    // Load edges points in a list

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
}

void DroneFactory::delete_vbos(){
    vbo_line.destroy();
    vbo_mesh.destroy();
}

void DroneFactory::init_shaders(){
    program_mesh->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSourceM);
    program_mesh->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSourceM);
    if (! program_mesh->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_mesh->log();
    }

    program_line->addShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSource);
    program_line->addShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSource);
    if (! program_line->link()) {  // édition de lien des shaders dans le shader program
        qWarning("Failed to compile and link shader program:");
        qWarning() << program_line->log();
    }
}

void DroneFactory::add_drone(int id_drone){
    QVector3D position_start = reader->accessPosition(id_drone, 0);
    position_start +=d;
    Drone d(position_start/40, 2.);
    d.init(program_mesh, program_line, vbo_mesh, vbo_line);
    drones.push_back(d);
}

void DroneFactory::draw(QMatrix4x4 projection, QMatrix4x4 view){
    //vbo_line.bind();
    //vbo_mesh.bind();
    for(Drone d: drones){
        //d.init(program_mesh, program_line, vbo_mesh, vbo_line);
        d.draw(projection, view, n_edges*2, n_faces*3);
    }
}


