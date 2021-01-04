#ifndef HELPER_H
#define HELPER_H

#include <drone.h>
//#include <dronefactory.h>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/ArrayKernel.hh>
#include <OpenMesh/Core/Geometry/Vector11T.hh>
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

class Helper
{
public:
    Helper();
    float distance(MyMesh::Point p1, MyMesh::Point p2);
    float diametre(MyMesh *_mesh);
    bool collision(Drone d1, Drone d2, float diametre);
    bool controle_vitesse(Drone drone, float vitesse_max);
};

#endif // HELPER_H
