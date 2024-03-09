#include <QLabel>
#include <QMouseEvent>
#include <iostream>

/* PROJECT LIBS */
#include <qcanvas.h>


void qcanvas::mouseMoveEvent(QMouseEvent *event)
{
    QPoint mouse = event->pos();
    // std::cerr << "debug(CANVAS): QLabel = [" << mouse.x() << "," << mouse.y() << "]\n";    
    emit user(mouse);
}