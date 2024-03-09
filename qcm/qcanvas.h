#ifndef QCANVAS_H
#define QCANVAS_H

#include <QWidget>
#include <QObject>
#include <QLabel>

using namespace std;

class qcanvas : public QLabel
{
    Q_OBJECT

    public:
        qcanvas() = default; // declare constructor to allow linker work    

    protected:
        void mouseMoveEvent(QMouseEvent *event) override;   

    signals:
        void user(QPoint mouse);    
        
};

#endif