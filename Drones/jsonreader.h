#ifndef JSONREADER_H
#define JSONREADER_H
#include <iostream>
#include <QString>
#include <QVariantMap>
class JsonReader
{
public:
    JsonReader(QString filePath);
    QString accessData(QString dataName);
    int get_nb_drones(){
        return nbDrones;
    }

    QVector3D accessPosition(int id_drone, int id_waypoint);
    int accesFrame(int id_drone, int id_waypoint);
    int get_nb_waypoints(int id_drone);
    QVariantMap get_waypoint_by_id(std::vector<QVariantMap> waypoints, int id);
    std::vector<QVector3D> compute_trajectory(int id_drone, QVector3D translation);
    std::vector<QVector3D> compute_vitesses(int id_drone, QVector3D translate);
    int get_framerate();
    int get_max_frame();
private:

    QVariantMap load_data();
    void show_waypoint();

    QString filename;
    QVariantMap jsonMap;
    QString description;
    std::vector<std::vector<QVariantMap>> drones;
    int nbDrones;
    int framerate;
};

#endif // JSONREADER_H
