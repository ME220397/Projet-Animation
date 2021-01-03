#ifndef HELPER_H
#define HELPER_H
#include<drone.h>
#include<dronefactory.h>
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Core/Mesh/ArrayKernel.hh>
#include <OpenMesh/Core/Geometry/Vector11T.hh>

class Helper
{
public:
    Helper();
    float distance(MyMesh::Point p1, MyMesh::Point p2);
    float rayon(MyMesh *_mesh);
    void collision();
    void controle_vitesse();
};

#endif // HELPER_H
