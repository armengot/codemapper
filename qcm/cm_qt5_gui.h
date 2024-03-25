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
#include <QTextEdit>

/* PROJECT LIBS */
#include <cm_graph.h>
#include <qcanvas.h>
#include <gitinfo.h>

using namespace std;

class cm_qt5_gui : public QMainWindow
{
    Q_OBJECT
    
    private:
        QString getversion();
        /* global vars */
        QScrollArea* sarea;
        QTextEdit* textbar;
        qcanvas* canvas;        
        string selected_input_language = "cpp";
        string selected_render_format = "png";
        bool lan_selected = false;

        /* target objects */
        cm_graph* current_project = nullptr;
        string svg;             
        
        /* setup */
        void setup_canvas();
        void setup_menus();
        /* input data load */
        void infolder();
        void infile();
        /* output data saving */
        void project_save();
        void guiabout();        
        void quitcool();
        void select_language(string lan);
        
    public:
        /* constructor */
        cm_qt5_gui();   
        
    protected:
        void closeEvent(QCloseEvent *event);
        void keyPressEvent(QKeyEvent *event) override;

    protected slots:
        void canvaslink(QPoint mouse);

};

#endif