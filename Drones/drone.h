#ifndef DRONE_H
#define DRONE_H
#include<QVector3D>

class Drone
{
public:
    Drone(QVector3D position, float size);
private:
    QVector3D position;
    float size;
};

#endif // DRONE_H
