#include "jsonreader.h"
#include <QFile>
#include <QByteArray>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>

using namespace std;

JsonReader::JsonReader(QString filePath)
{
    filename = filePath;
    jsonMap = load_data();
    // Get the description string
    description = jsonMap["description"].toString();
    qDebug() << description;

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
    show_waypoint();
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
