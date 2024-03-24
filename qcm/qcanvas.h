#ifndef QCANVAS_H
#define QCANVAS_H

/* Qt libraries */
#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QSvgRenderer>
#include <QPolygonF>
#include <QTextEdit>

/* external standard libraries */
#include <iostream>

/* codemapper project libraries */
#include <cm_graph.h>

using namespace std;

struct xmlnode
{
    string id;
    string title;
    string label;
    QPointF center;        
    QPolygonF bounding;
};

struct xmlsvg
{
    int ptw;
    int pth;
    float vbox[4];
    int scale[2];
    float rotate;
    float translate[2];
    vector<xmlnode> nodes;
};

void debugqt(std::string stin);

class qcanvas : public QLabel
{
    Q_OBJECT

    public:
        qcanvas() = default; // declare constructor to allow linker work            
        void load(std::string svg);
        void setgraph(cm_graph* project);
        int xmlingest(std::string svg);
        void deletenode();
        QTextEdit* canvas_textline;
                

    protected:
        void mouseMoveEvent(QMouseEvent *event) override;   
        void mousePressEvent(QMouseEvent *event) override;
        //void keyPressEvent(QKeyEvent *event) override;

    private:
        /* functions */
        void select_node(std::string label);
        /* attributes*/
        bool svg_loaded_as_xml = false;
        QPoint mouse;
        QSvgRenderer svg_render;
        QString xml;
        struct xmlsvg currentsvg;
        cm_graph* current_project;
        string selected_node = "";

    signals:
        void user(QPoint mouse);    
        
};

#endif