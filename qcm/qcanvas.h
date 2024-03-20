#ifndef QCANVAS_H
#define QCANVAS_H

/* Qt libraries */
#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QSvgRenderer>
#include <QPolygonF>

/* external standard libraries */
#include <iostream>

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
        int xmlingest(std::string svg);
                

    protected:
        void mouseMoveEvent(QMouseEvent *event) override;   

    private:
        bool svg_loaded_as_xml = false;
        QPoint mouse;
        QSvgRenderer svg_render;
        QString xml;
        struct xmlsvg currentsvg;

    signals:
        void user(QPoint mouse);    
        
};

#endif