/* external standar */
#include <string>
#include <iostream>
#include <regex>

/* Qt5 environment libraries */
#include <QLabel>
#include <QMouseEvent>
#include <QSvgRenderer>
#include <QString>

/* PROJECT LIBS */
#include <qcanvas.h>

void qcanvas::mouseMoveEvent(QMouseEvent *event)
{
    mouse = event->pos();
    //std::cerr << "debug(CANVAS): QLabel = [" << mouse.x() << "," << mouse.y() << "]\n";    
    if (svg_render)
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
    }
    emit user(mouse);
}

void qcanvas::set_render(QSvgRenderer* svg)
{
    svg_render = svg;
}

void qcanvas::xmlingest()
{
    /* parse xml */
    bool ok = true;
    QXmlStreamReader xmlReader(xml);    

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

                }
            }
        }
    }

}