#ifndef DRONEFACTORY_H
#define DRONEFACTORY_H

#include<iostream>
#include<QVariantMap>

#include <QOpenGLWidget>
#include <QOpenGLBuffer>
#include <QOpenGLShaderProgram>
#include <QOpenGLFunctions>
#include <jsonreader.h>
#include <drone.h>

#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <QGLWidget>
#include "QMouseEvent"
#include <OpenMesh/Core/Geometry/VectorT.hh>

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

using namespace OpenMesh;
using namespace OpenMesh::Attributes;

struct MyTraits : public OpenMesh::DefaultTraits
{
    // use vertex normals and vertex colors
    VertexAttributes( OpenMesh::Attributes::Normal | OpenMesh::Attributes::Color );
    // store the previous halfedge
    HalfedgeAttributes( OpenMesh::Attributes::PrevHalfedge );
    // use face normals face colors
    FaceAttributes( OpenMesh::Attributes::Normal | OpenMesh::Attributes::Color );
    EdgeAttributes( OpenMesh::Attributes::Color );
    // vertex thickness
    VertexTraits{float thickness; float value; Color faceShadingColor;};
    // edge thickness
    EdgeTraits{float thickness;};
};
typedef OpenMesh::TriMesh_ArrayKernelT<MyTraits> MyMesh;
class DroneFactory
{
public:
    DroneFactory(QOpenGLWidget * parent);
    void resetAllColorsAndThickness(MyMesh* _mesh);
    void loadMesh();
    void loadLines(MyMesh *_mesh);
    void delete_vbos();
    void init_shaders();
    void add_drone(int id_drone);
    void draw(QMatrix4x4 projection, QMatrix4x4 view);
    void create_drones();
    void draw_trajectories(QMatrix4x4 projection, QMatrix4x4 view);
private:

    std::vector<Drone> drones;

    int n_faces;
    int n_edges;
    int n_lines_trajectory;
    QOpenGLBuffer vbo_mesh;
    QOpenGLBuffer vbo_line;
    QOpenGLBuffer vbo_trajectory;
    QOpenGLShaderProgram *program_mesh; // pour les faces du drone
    QOpenGLShaderProgram *program_line; // pour les arête des drones
    QOpenGLShaderProgram *program_trajectory; // pour les lignes de trajectoire
    MyMesh mesh;

    JsonReader *reader;
    int nb_drones;
    QVector3D d;

    std::vector<std::vector<QVector3D>> trajectories;
};

#endif // DRONEFACTORY_H
