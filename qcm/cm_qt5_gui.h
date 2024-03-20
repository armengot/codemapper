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
#include <QSvgRenderer>
#include <QString>
#include <QXmlStreamReader>

/* PROJECT LIBS */
#include <cm_graph.h>
#include <qcanvas.h>
#include <gitinfo.h>

using namespace std;

class cm_qt5_gui : public QMainWindow
{
    Q_OBJECT
    
    private:
        /* global vars */
        QScrollArea* sarea;
        
        qcanvas* canvas;        
        string selected_input_language = "cpp";
        string selected_render_format = "png";

        /* target objects */
        cm_graph* current_project;
        std::string svg;             

        /* general */
        void render();
        
        /* setup */
        void setup_canvas();
        void setup_menus();
        void infolder();
        void infile();
        void guiabout();
        void quitcool();
        QString getversion();

    public:
        /* constructor */
        cm_qt5_gui();    

    protected:
        void closeEvent(QCloseEvent *event);

    protected slots:
        void canvaslink(QPoint mouse);

};

#endif