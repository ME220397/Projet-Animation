// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "princ.h"
#include <QApplication>
#include <jsonreader.h>
#include <QDir>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Princ w;
    w.show();
    JsonReader js("./json/waypoints.json");
    return a.exec();
}
