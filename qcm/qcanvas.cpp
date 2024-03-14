/* external standar */
#include <string>
#include <iostream>
#include <regex>

/* Qt5 environment libraries */
#include <QLabel>
#include <QMouseEvent>
#include <QSvgRenderer>
#include <QString>
#include <QPainter>
#include <QByteArray>
#include <QXmlStreamReader>

/* PROJECT LIBS */
#include <qcanvas.h>

void debugqt(std::string stin)
{
    cerr << "debug(QT): " << stin << endl; 
}

void qcanvas::mouseMoveEvent(QMouseEvent *event)
{
    mouse = event->pos();
    //std::cerr << "debug(CANVAS): QLabel = [" << mouse.x() << "," << mouse.y() << "]\n";    
    if (svg_loaded_as_xml)
    {
        /* ratio between CANVAS and original SVG */
        QRect frame = geometry();        
        /* mouse relative to canvas */
        float rx = static_cast<float>(mouse.x()) / frame.width();
        float ry = static_cast<float>(mouse.y()) / frame.height();
        //std::cerr << "MOUSE(" << mouse.x() << "," << mouse.y() << ") -> RELATIVE (" << rx << "," << ry << ") ";
        /* qx qy = scale */
        float qx = rx * currentsvg.vbox[2];
        float qy = ry * currentsvg.vbox[3];
        //std::cerr << "SVG[" << qx << "," << qy << "]";
        /* de-translation */
        QPointF svgpos;
        svgpos.setX(qx - currentsvg.translate[0]);
        svgpos.setY(qy - currentsvg.translate[1]);
        //std::cerr << " ==> [" << svgpos.x() << "," << svgpos.y() << "]\n";     
        for (const auto& square: currentsvg.nodes)
        {
            if (square.bounding.contains(svgpos))
            {
                std::cerr << square.label << std::endl;
            }
            else
            {
                /*
                std::cerr << "[" << svgpos.x() << "," << svgpos.y() << "] outside of {";
                for (const QPointF& v : square.bounding)
                {
                    std::cerr << "(" << v.x() << "," << v.y() << ") ";
                }
                std::cerr << std::endl;
                */
            }
        }
    }
    emit user(mouse);
}

void qcanvas::load(std::string svg)
{    
    //std::cerr << svg;            
    QByteArray qbytes(svg.c_str(), static_cast<int>(svg.length()));        
    svg_render.load(qbytes);

    if (svg_render.isValid()) 
    {
        QPixmap pixmap(svg_render.defaultSize());
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        svg_render.render(&painter);
        painter.end();    
        setPixmap(pixmap);                
        if (xmlingest(svg)>2)
            svg_loaded_as_xml = true;
    }
    else 
    {
        debugqt("Error happened rendering SVG graph.");
    }
}

int qcanvas::xmlingest(std::string svg)
{
    /* parse xml */
    xml = QString::fromStdString(svg);        
    QXmlStreamReader xmlReader(xml);
    bool ok = true;
    int r = 0;

    while (!xmlReader.atEnd() && !xmlReader.hasError()) 
    {
        xmlReader.readNext();

        if (xmlReader.isStartElement() && xmlReader.name() == "svg")
        {
            currentsvg.ptw = xmlReader.attributes().value("width").toString().chopped(2).toInt();
            currentsvg.pth = xmlReader.attributes().value("height").toString().chopped(2).toInt();
            std::cerr << currentsvg.ptw << " x " << currentsvg.pth << std::endl;
            QStringList four = xmlReader.attributes().value("viewBox").toString().split(' ');
            for (int i=0;i<4;i++)
            {
                currentsvg.vbox[i] = four[i].toFloat(&ok);
            }
            std::cerr << currentsvg.vbox[0] << " " << currentsvg.vbox[1] << " " << currentsvg.vbox[2] << " " << currentsvg.vbox[3] << std::endl;
            r = r + 1;            
        }
        if (xmlReader.name() == "g")
        {
            if (xmlReader.attributes().value("id").toString() == "graph0")
            {
                QString transform = xmlReader.attributes().value("transform").toString();
                std::string tvalues = transform.toStdString();
                //std::cerr << "Transform: " << transform.toStdString() << std::endl;
                std::regex regex("scale\\((-?\\d+\\.?\\d*) (-?\\d+\\.?\\d*)\\) rotate\\((-?\\d+\\.?\\d*)\\) translate\\((-?\\d+\\.?\\d*) (-?\\d+\\.?\\d*)\\)");
    
                std::smatch match;
                if (std::regex_search(tvalues, match, regex)) 
                {
                    currentsvg.scale[0] = std::stof(match[1].str());
                    currentsvg.scale[1] = std::stof(match[2].str());
                    currentsvg.rotate   = std::stof(match[3].str());
                    currentsvg.translate[0] = std::stof(match[4].str());
                    currentsvg.translate[1] = std::stof(match[5].str());                    
                    std::cerr << "scale: " << currentsvg.scale[0] << ":"<< currentsvg.scale[1];
                    std::cerr << " rotate: " << currentsvg.rotate;
                    std::cerr << " translate: [" << currentsvg.translate[0] << "," << currentsvg.translate[1] << "]" << std::endl;
                    r = r + 1;
                }
            }        
            else
            {                
                if (xmlReader.attributes().value("class").toString().toStdString()=="node")
                {       
                    xmlnode newn;
                    newn.id = xmlReader.attributes().value("id").toString().toStdString();

                    xmlReader.readNext();                    
                    while (xmlReader.name().toString().toStdString() != "title")
                        xmlReader.readNext();
                    newn.title = xmlReader.readElementText().toStdString();
                    xmlReader.readNext();                    
                    while (xmlReader.name() != "text")
                        xmlReader.readNext();                    
                    newn.label = xmlReader.readElementText().toStdString();
                    while (xmlReader.name() != "polygon") 
                        xmlReader.readNext();                
                    QString pointstring = xmlReader.attributes().value("points").toString();                    
                    QStringList lpoints = pointstring.split(' ');
                    for (const QString& point : lpoints)
                    {                        
                        QStringList coords = point.split(',');
                        if (coords.size() == 2)
                        {
                            qreal x = coords[0].toFloat();
                            qreal y = coords[1].toFloat();
                            newn.bounding.append(QPointF(x, y));
                        }
                    }
                    currentsvg.nodes.push_back(newn);
                    std::cerr << newn.id << " " << newn.title << " " << newn.label << std::endl;
                    for (auto each : newn.bounding)
                    {
                        std::cerr << "\t" << each.x() << " " << each.y() << std::endl;
                    }
                    r = r + 1;
                }
            }
        }        
    }    
    return(r);
}