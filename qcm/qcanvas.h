#ifndef QCANVAS_H
#define QCANVAS_H

#include <QWidget>
#include <QObject>
#include <QLabel>
#include <QSvgRenderer>

using namespace std;

struct xmlnode
{
    string id;
    string title;
    string label;    
    QVector<QPointF> bounding;
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

class qcanvas : public QLabel
{
    Q_OBJECT

    public:
        qcanvas() = default; // declare constructor to allow linker work    
        void set_render(QSvgRenderer* svg);
        void xmlingest();
        QString xml;        

    protected:
        void mouseMoveEvent(QMouseEvent *event) override;   

    private:
        QPoint mouse;
        QSvgRenderer* svg_render = nullptr;
        struct xmlsvg currentsvg;

    signals:
        void user(QPoint mouse);    
        
};

#endif