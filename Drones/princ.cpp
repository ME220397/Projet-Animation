// Basé sur :
// CC-BY Edouard.Thiel@univ-amu.fr - 22/01/2019

#include "princ.h"
#include <QDebug>

Princ::Princ(QWidget *parent) : QMainWindow(parent)
{
    setupUi(this);
    connect(pushButton, SIGNAL(clicked()), glarea, SLOT(on_push_trajectory()));
    connect(axis, SIGNAL(clicked()), glarea, SLOT(on_push_axis()));
    connect(repere, SIGNAL(clicked()), glarea, SLOT(on_push_repere()));
    connect(sol, SIGNAL(clicked()), glarea, SLOT(on_push_sol()));
}
