#include "helper.h"

Helper::Helper()
{

}

float Helper::distance(MyMesh::Point p1, MyMesh::Point p2){
    return sqrt(pow((p1[0] - p2[0]),2) + pow(p1[1] - p2[1], 2) + pow(p1[2] - p2[2], 2));
}
float Helper::rayon(MyMesh *_mesh){
    MyMesh::Point  max_coord = MyMesh::Point(0, 0, 0);
    MyMesh::Point  min_coord = MyMesh::Point(100000000, 10000000, 10000000);
    for(MyMesh::VertexIter v_it = _mesh->vertices_begin(); v_it != _mesh->vertices_end(); ++v_it)
    {
        VertexHandle vh = *v_it;
        MyMesh::Point p = _mesh->point(vh);
        if(max_coord[0] < p[0]){
            max_coord[0] = p[0];
        }
        if(max_coord[1] < p[1]){
            max_coord[1] = p[1];
        }
        if(max_coord[2] < p[2]){
            max_coord[2] = p[2];
        }
        if(min_coord[0] > p[0]){
            min_coord[0] = p[0];
        }
        if(min_coord[1] > p[1]){
            min_coord[1] = p[1];
        }
        if(min_coord[2] > p[2]){
            min_coord[2] = p[2];
        }
    }
    float max_dist = sqrt(pow((min_coord[0] - max_coord[0]),2));
    if(max_dist < sqrt(pow((min_coord[1] - max_coord[1]),2)))
        max_dist = sqrt(pow((min_coord[1] - max_coord[1]),2));
    if(max_dist < sqrt(pow((min_coord[2] - max_coord[2]),2)))
        max_dist = sqrt(pow((min_coord[2] - max_coord[2]),2));
    return max_dist; //Peut etre utiliser la fonction distance
}
void Helper::collision(){

}
void Helper::controle_vitesse(){

}














