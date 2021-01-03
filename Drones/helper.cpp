#include "helper.h"

Helper::Helper()
{

}


float Helper::distance(MyMesh::Point p1, MyMesh::Point p2){
    return sqrt(pow((p1[0] - p2[0]),2) + pow(p1[1] - p2[1], 2) + pow(p1[2] - p2[2], 2));
}

float Helper::diametre(MyMesh *_mesh){
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
    float max_dist = distance(min_coord, max_coord);
    return max_dist; //Renvoie le rayon *2(le diametre
}

bool Helper::collision(Drone d1, Drone d2, float diametre){
    QVector3D D1;
    QVector3D D2;
    D1 = d1.get_position();
    D2 = d2.get_position();
    MyMesh::Point p1(D1[0], D1[1], D1[2]);
    MyMesh::Point p2(D2[0], D2[1], D2[2]);
    if(distance(p1, p2) < diametre){
        return true;
    }
    return false;
}

bool Helper::controle_vitesse(Drone drone, float vitesse_max){//true -> trop rapide / false -> aucun soucis
    QVector3D vitesse = drone.get_vitesse();
    if(vitesse[0] > vitesse_max || vitesse[1] > vitesse_max || vitesse[2] > vitesse_max){
        return true;//Trop rapide
    }
    return false;
}














