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
    connect(play, SIGNAL(clicked()), glarea, SLOT(on_push_play()));
    connect(pause, SIGNAL(clicked()), glarea, SLOT(on_push_pause()));

    connect(glarea, SIGNAL(setRangeTL(int, int)), timeline, SLOT(setRange(int, int)));
    connect(glarea, SIGNAL(setFrame(int)), timeline, SLOT(setValue(int)));
    connect(timeline, SIGNAL(valueChanged(int)), glarea, SLOT(frameChanged(int)));

    int max = glarea->get_max_frame();
    glarea->setRangeTL(0, max-1);

    int current_frame = glarea->get_frame();
    glarea->setFrame(current_frame);
}
