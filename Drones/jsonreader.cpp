#include "jsonreader.h"
#include <QFile>
#include <QByteArray>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QVector3D>

using namespace std;

JsonReader::JsonReader(QString filePath)
{
    filename = filePath;
    jsonMap = load_data();
    // Get the description string
    description = jsonMap["description"].toString();
    qDebug() << description;

    // Récupération du frameRate
    framerate = jsonMap["framerate"].toInt();

   //Get the list of item for "drones"
    QList<QVariant> list = jsonMap["drones"].toList();
    nbDrones = list.length();

     //Get a list of waypoints as QVariantMap
    for(QVariant v : list){
        QVariantMap items = v.toMap();
        QList<QVariant> waypoints = items["waypoints"].toList();
        std::vector<QVariantMap> tovect;
        for(QVariant waypoint : waypoints){
            tovect.push_back(waypoint.toMap());
        }
        drones.push_back(tovect);
    }
    int n = drones.size();
    assert(n == nbDrones);
    //show_waypoint();
}

QVariantMap JsonReader::load_data(){
    //Step 1 : Load the file and convert its contents into a QbyteQrray
    QFile jsonFile(filename);

    assert(jsonFile.open(QIODevice::ReadOnly));

    QTextStream fileContent(&jsonFile);
    QString jsonString = fileContent.readAll();
    jsonFile.close();
    QByteArray jsonByte = jsonString.toLocal8Bit();

    //Step 2 : Load the byte content into a QJSonDocument then to QJsonObject
    auto jsonDoc = QJsonDocument::fromJson(jsonByte);

    assert(!jsonDoc.isNull());
    assert(jsonDoc.isObject());

    QJsonObject jsonObj = jsonDoc.object();

    assert(!jsonObj.isEmpty());

    QVariantMap jsonMap = jsonObj.toVariantMap();
    assert(!jsonMap.isEmpty());

    return jsonMap;
}

void JsonReader::show_waypoint(){
    int count = 0;
    for(std::vector<QVariantMap> vec : drones){
        cout << "id = " << nbDrones - count << endl;
        for(QVariantMap wp : vec){
            cout << "\tframe : " << wp["frame"].toInt() << endl;
            cout << "\tid : " << wp["id"].toInt() << endl;
            cout << "\torder : " << wp["order"].toInt() << endl;
        }
        cout << "\n";
        count++;
    }
}

QVariantMap JsonReader::get_waypoint_by_id(std::vector<QVariantMap> waypoints, int id){
    return waypoints[id];
}

QVector3D JsonReader::accessPosition(int id_drone, int id_waypoint){
    // Nous voulons acceder aux drones dans l'ordre croissant et non pas
    // en commançant pas 20 comme dans le json
    int id = (nbDrones-1) - id_drone;
    // On récupère le waypoint associé au drone
    QVariantMap waypoint = get_waypoint_by_id(drones[id], id_waypoint);
    // On recupère la map de la position
    QVariantMap position = waypoint["position"].toMap();

    // On récupère les coordonnées de la position
    float x, y, z;

    x = position["lng_X"].toFloat();
    y = position["alt_Y"].toFloat();
    z = position["lat_Z"].toFloat();

    return QVector3D(x, y ,z);
}

vector<QVector3D> JsonReader::compute_trajectory(int id_drone, QVector3D translate){
    int id = (nbDrones-1) - id_drone;
    vector<QVariantMap> waypoints = drones[id];

    vector<QVector3D> trajectory;

    for(int i = 0; i<(int)waypoints.size(); i++){
        QVector3D position = accessPosition(id_drone, i);
        position += translate;
        trajectory.push_back(position/40);
    }

    return trajectory;
}

int JsonReader::accesFrame(int id_drone, int id_waypoint){
    // Nous voulons acceder aux drones dans l'ordre croissant et non pas
    // en commançant pas 20 comme dans le json
    int id = (nbDrones-1) - id_drone;
    // On récupère le waypoint associé au drone
    QVariantMap waypoint = get_waypoint_by_id(drones[id], id_waypoint);

    // On recupere le frame associé au waypoint
    int frame = waypoint["frame"].toInt();

    return frame;
}

int JsonReader::get_nb_waypoints(int id_drone){
    // Nous voulons acceder aux drones dans l'ordre croissant et non pas
    // en commançant pas 20 comme dans le json
    int id = (nbDrones-1) - id_drone;
    // On récupère le nombre de waypoints pour le drone id
    int n = drones[id].size();

    return n;
}

int JsonReader::get_framerate(){
    return framerate;
}
