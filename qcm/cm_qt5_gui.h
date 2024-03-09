/* ------------------ */
/* codemapper Qt5 gui */
/* ------------------ */

#ifndef CMQT5GUI_H
#define CMQT5GUI_H

/* STANDARD LIBS */
#include <iostream>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QMainWindow>
#include <QScrollArea>
#include <QLabel>
#include <QObject>

/* PROJECT LIBS */
#include <qcanvas.h>

using namespace std;

class cm_qt5_gui : public QMainWindow
{
    Q_OBJECT
    
    private:
        /* global vars */                
        qcanvas *canvas;        

        /* debug */
        void debugqt(std::string stin);        

        /* general */
        void render();
        
        /* setup */
        void setup_canvas();
        void setup_menus();
        void infolder();
        void infile();
        void guiabout();
        void quitcool();

    public:
        /* constructor */
        cm_qt5_gui();    

    /*
    protected:
        void closeEvent(QCloseEvent *event);

    protected slots:
        void canvaslink(QPoint mouse);
    */
};

#endif