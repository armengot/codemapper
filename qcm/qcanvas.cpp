/* external standar */
#include <string>
#include <iostream>
#include <regex>
#include <math.h>

/* Qt5 environment libraries */
#include <QLabel>
#include <QKeySequence>
#include <QMouseEvent>
#include <QSvgRenderer>
#include <QString>
#include <QPainter>
#include <QByteArray>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QMessageBox>
#include <QMenu>

/* codemapper project libraries */
#include <cm_qt5_gui.h>
#include <qcanvas.h>
#include <tools.h>

void debugqt(std::string stin)
{
    cerr << "debug(QT): " << stin << endl; 
}

void qcanvas::mousePressEvent(QMouseEvent *event)
{
    callable_rightmouse(event);
}

void qcanvas::callable_rightmouse(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) 
    {
        debugqt("LEFT");
        QString to_find = canvas_textline->toPlainText();
        if (selected_node != to_find.toStdString())
        {
            
            if (!to_find.isEmpty()) // case mouse over label
            {
                select_node(to_find.toStdString());
                selected_edge = nullptr;
                selected_node = to_find.toStdString();            
                lastclick = event->localPos().toPoint();
                std::cerr << "qcanvas::callable_rightmouse: Selected node: " << selected_node << std::endl;
            }
            else
            {                       // mouse everywhere != label
                selected_node = "";
                load(xml.toStdString());           
                std::cerr << "qcanvas::callable_rightmouse: Any node selected node." << std::endl;
            }
        }
        else
        {
            cm_node* p = current_project->lookfor(selected_node);
            if (p != nullptr)
            {
                QMenu *cm_node_features = new QMenu(this);
                QAction *action;
                for (auto& each : p->get_features())
                {
                    std::string feature = static_cast<string>(each);
                    action = new QAction(QString::fromStdString(feature), this);
                    cm_node_features->addAction(action);
                }
                cm_node_features->popup(mapToGlobal(QPoint(0,0))+lastclick);
            }
        }        
    }
    else if (event->button() == Qt::RightButton) 
    {
        debugqt("RIGHT");        
        if (selected_node!="")
        {            
            cm_node* p = current_project->lookfor(selected_node);
            std::vector<cm_edge*> involved_edges = current_project->edgesinvolved(p);
            if (!involved_edges.empty())
            {
                QMenu *cm_popup = new QMenu(this);
                QAction *action;
                for (auto& each_edge : involved_edges)
                {                    
                    debugqt("qcanvas: "+each_edge->humanreadable());
                    action = new QAction(QString::fromStdString(each_edge->humanreadable()), this);
                    cm_edge* edge_ptr = static_cast<cm_edge*>(each_edge);                    
                    QObject::connect(action, &QAction::triggered, this, [this, edge_ptr]() { this->select_edge(edge_ptr); });   
                    cm_popup->addAction(action);
                }
                
                cm_popup->popup(mapToGlobal(QPoint(0,0))+lastclick);                
            }
        }
        else
        {
            if (selected_edge != nullptr)
            {                
                QMenu *cm_edge_features = new QMenu(this);
                QAction *action;
                for (auto& each : selected_edge->get_features())
                {
                    std::string feature = static_cast<string>(each);
                    action = new QAction(QString::fromStdString(feature), this);
                    cm_edge_features->addAction(action);
                }
                cm_edge_features->popup(mapToGlobal(QPoint(0,0))+lastclick);                
            }        
        }                
    } 
    else if (event->button() == Qt::MiddleButton) 
    {
        debugqt("CENTER");        
    }    
    QLabel::mousePressEvent(event);
}

void qcanvas::select_edge(cm_edge* direct)
{
    debugqt("qcanvas::select_edge "+direct->humanreadable());
    current_project->reset_edge_colors();
    direct->setcolor("blue");
    std::string new_svg,fromgraphviz = current_project->to_string();
    cm_dashclean(fromgraphviz);
    cm_render(fromgraphviz, new_svg, CM_OUTPUT_SVG);                
    load(new_svg);
    selected_edge = direct;
    cm_node* tailedge = direct->get_tail();
    if (tailedge->get_name() == selected_node)
    {
        tailhead = 0;
    }
    else
    {
        tailhead = 1;
    }
    selected_node = "";       
}

void qcanvas::deleteedge()
{
    if (selected_edge != nullptr)
    {
        current_project->removeedge(selected_edge);
        std::string new_svg,fromgraphviz = current_project->to_string();
        cm_dashclean(fromgraphviz);
        cm_render(fromgraphviz, new_svg, CM_OUTPUT_SVG);
        svg_loaded_as_xml = false; // force re ingest                
        load(new_svg);        
    }
    else
    {
        debugqt("No selected edge to remove.");
    }
}

void qcanvas::deletenode()
{
    if (selected_node != "")
    {
        std::string clean = selected_node;
        if (charin(CM_SYS_SPLITER_CHAR,clean))
        {
            rechar(clean,CM_SYS_SPLITER_CHAR,CM_GLOBAL_JOIN_CHAR);
        }
        cm_dashclean(clean);
        debugqt("Removing node: "+selected_node+" --> parsed as: "+clean);
        current_project->removenode(clean);
        std::string new_svg,fromgraphviz = current_project->to_string();
        cm_dashclean(fromgraphviz);
        cm_render(fromgraphviz, new_svg, CM_OUTPUT_SVG);
        svg_loaded_as_xml = false;                
        load(new_svg);
        //xmlingest(new_svg);        
        //xml = QString::fromStdString(new_svg);        
    }
    else
    {
        debugqt("No selected node to remove.");
    }
}


void qcanvas::mouseMoveEvent(QMouseEvent *event)
{
    mouse = event->pos();
    //std::cerr << "debug(CANVAS): QLabel = [" << mouse.x() << "," << mouse.y() << "]\n";    
    if (svg_loaded_as_xml)
    {
        /* *********************************************************************************** */        
        /* Step (1) convert mouse coords to real SVG world coords */
        /* *********************************************************************************** */
        /* ratio between CANVAS and original SVG */
        QRect frame = geometry();        
        /* mouse relative to canvas */
        float rx = static_cast<float>(mouse.x()) / frame.width();
        float ry = static_cast<float>(mouse.y()) / frame.height();
        //std::cerr << "MOUSE(" << mouse.x() << "," << mouse.y() << ") -> RELATIVE (" << rx << "," << ry << ") ";
        /* qx qy = scale */
        float qx = rx * currentsvg->vbox[2];
        float qy = ry * currentsvg->vbox[3];
        //std::cerr << "SVG[" << qx << "," << qy << "]";
        /* de-translation */
        QPointF svgpos;
        svgpos.setX(qx - currentsvg->translate[0]);
        //svgpos.setY(currentsvg->translate[1] - qy);
        svgpos.setY(qy - currentsvg->translate[1]);
        //std::cerr << " ==> [" << svgpos.x() << "," << svgpos.y() << "]\n";     

        /* *********************************************************************************** */        
        /* Step (2) look for eastest near node under a threshold */
        /* *********************************************************************************** */        
        bool text_bar_filled = false;
        for (const auto& square: currentsvg->nodes)
        {
            qreal diffX = square.center.x() - svgpos.x();
            qreal diffY = square.center.y() - svgpos.y();
            qreal distance = sqrt(diffX * diffX + diffY * diffY);            
            //std::cerr << "\t" << square.label << "[" << square.center.x() << "," << square.center.y() << "] : " << distance << std::endl;
            if (distance<20)
            {
                // std::cerr << square.label << std::endl;
                canvas_textline->setText(QString::fromStdString(square.label));
                text_bar_filled = true;
            }
        }
        if (!text_bar_filled)
        {
            /* always cleaning the status bar */
            canvas_textline->setText(QString::fromStdString(""));
        }
    }
    emit user(mouse);
}

void qcanvas::setgraph(cm_graph* project)
{
    current_project = project;
}

void qcanvas::load(std::string svg)
{    
    // std::cout << svg;            
    QByteArray qbytes(svg.c_str(), static_cast<int>(svg.length()));        
    svg_render.load(qbytes);

    std::cerr << DEBUG_GRNTXT << "REFRESH";
    if (svg_render.isValid()) 
    {
        std::cerr << DEBUG_GRNTXT << " (OK)" << DEBUG_RESTXT << std::endl;
        QPixmap pixmap(svg_render.defaultSize());
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        svg_render.render(&painter);
        painter.end();    
        setPixmap(pixmap);   
        if (!svg_loaded_as_xml) // first time
        {
            if (xmlingest(svg)>1)
            {
                svg_loaded_as_xml = true;
            }
            else
            {
                if (currentsvg!=nullptr)
                    delete(currentsvg);                    
                currentsvg = nullptr;
                current_project = nullptr;
                QMessageBox warning;
                warning.setWindowTitle("Warning");
                warning.setText("Any node was recover from input folder, check language selection.");
                warning.setStandardButtons(QMessageBox::Ok);    
                warning.exec();
            }
        }
    }
    else 
    {
        std::cerr << DEBUG_REDTXT << " (wrong   )" << DEBUG_RESTXT << std::endl;
        debugqt("Error happened rendering SVG graph.");
    }
}

void qcanvas::select_node(std::string label)
{
    /* *********************** */
    bool found = false;
    QXmlStreamReader reader(xml);
    QString xmlout;
    QXmlStreamWriter writer(&xmlout);    
    writer.setCodec("UTF-8");
    writer.setAutoFormatting(true);
    std::string target,debugger;
    QString qtarget;
    QString tqvoid = QString("");
    /* *********************** */
    if (charin('.',label))    
    {    
        target = firstname(label,'.');
    }
    else
    {
        target = label;
    }
    if (charin(CM_SYS_SPLITER_CHAR,target))        
    {        
        rechar(target,CM_SYS_SPLITER_CHAR,CM_GLOBAL_JOIN_CHAR);
    }
    std::cerr << "TARGET: " << target << std::endl;
    qtarget = QString::fromStdString(target);
    reader.setNamespaceProcessing(false);    
    /* remember:
    <!-- cm_lan -->
    <g id="node1" class="node">
    <title>cm_lan</title>
    <polygon fill="none" stroke="black" points="281.69,-260.3 281.69,-309.9 361.69,-309.9 361.69,-260.3 281.69,-260.3"/>
    <text text-anchor="middle" x="321.69" y="-292.6" font-family="Times,serif" font-size="14.00">cm_lan.cpp</text>
    <polyline fill="none" stroke="black" points="281.69,-285.1 361.69,-285.1"/>
    <text text-anchor="middle" x="321.69" y="-267.8" font-family="Times,serif" font-size="14.00">cm_lan.h</text>
    </g> */
    while (!reader.atEnd() && !reader.hasError()) 
    {
        reader.readNext(); 
        debugger = reader.name().toString().toStdString();
        if (reader.isStartElement()) 
        {        
            if (reader.attributes().value("class").toString() == "node") 
            {
                while (!(reader.name() == "title"))
                {
                    writer.writeCurrentToken(reader);
                    reader.readNext();
                    debugger = reader.name().toString().toStdString();
                }
                if (!reader.isStartElement())
                {
                    writer.writeCurrentToken(reader);
                }
                else
                {   
                    // std::cerr << "\n\n" << xmlout.toStdString() << std::endl;
                    writer.writeCurrentToken(reader);
                    QString current = reader.readElementText();
                    writer.writeCharacters(current);
                    //std::cerr << "\n\n" << xmlout.toStdString() << std::endl;
                    writer.writeCurrentToken(reader);
                    //std::cerr << "\n\n" << xmlout.toStdString() << std::endl;
                    //std::cerr << "\tcomparing " << qtarget.toStdString() << " with " << current.toStdString() << std::endl;
                    //if (current.indexOf(qtarget) != -1)     
                    //if (current.toStdString() == qtarget.toStdString())                     
                    if (current == qtarget)
                    {                           
                        //std::cerr << "\n\n" << xmlout.toStdString() << std::endl;
                        //std::cerr << "\t\tINSIDE: " << reader.name().toString().toStdString() << std::endl;
                        reader.readNext();
                        while (!(reader.name() == "polygon")) 
                        {
                            writer.writeCurrentToken(reader);
                            //std::cerr << "\n\n" << xmlout.toStdString() << std::endl;
                            reader.readNext();
                            debugger = reader.name().toString().toStdString();
                        }
                        //std::string foo = reader.name().toString().toStdString();                        
                        writer.writeStartElement(reader.name().toString());                        
                        writer.writeAttribute("stroke-width", "3");                        
                        writer.writeAttributes(reader.attributes());                        
                        //writer.writeEndElement();
                        //reader.readNext();
                    }
                    else
                    {
                        //writer.writeCurrentToken(reader);
                    }
                }
            }
            else
            {
                writer.writeCurrentToken(reader);    
            }
        }   
        else
        {
            writer.writeCurrentToken(reader);
        }
    }    
    //std::cout << xmlout.toStdString() << std::endl;
    load(xmlout.toStdString());
}

int qcanvas::xmlingest(std::string svg)
{
    /* parse xml */
    xml = QString::fromStdString(svg);        
    QXmlStreamReader reader(xml);
    bool ok = true;
    int r = 0;

    debugqt("qcanvas::xmlingest: starting render and ingest");
    if (currentsvg)
    {
        delete(currentsvg);
    }
    currentsvg = new xmlsvg;

    while (!reader.atEnd() && !reader.hasError()) 
    {
        reader.readNext();

        if (reader.isStartElement() && reader.name() == "svg")
        {
            currentsvg->ptw = reader.attributes().value("width").toString().chopped(2).toInt();
            currentsvg->pth = reader.attributes().value("height").toString().chopped(2).toInt();
            std::cerr << currentsvg->ptw << " x " << currentsvg->pth << std::endl;
            QStringList four = reader.attributes().value("viewBox").toString().split(' ');
            for (int i=0;i<4;i++)
            {
                currentsvg->vbox[i] = four[i].toFloat(&ok);
            }
            std::cerr << currentsvg->vbox[0] << " " << currentsvg->vbox[1] << " " << currentsvg->vbox[2] << " " << currentsvg->vbox[3] << std::endl;
            r = r + 1;            
        }
        if (reader.name() == "g")
        {
            if (reader.attributes().value("id").toString() == "graph0")
            {
                QString transform = reader.attributes().value("transform").toString();
                std::string tvalues = transform.toStdString();
                //std::cerr << "Transform: " << transform.toStdString() << std::endl;
                std::regex regex("scale\\((-?\\d+\\.?\\d*) (-?\\d+\\.?\\d*)\\) rotate\\((-?\\d+\\.?\\d*)\\) translate\\((-?\\d+\\.?\\d*) (-?\\d+\\.?\\d*)\\)");
    
                std::smatch match;
                if (std::regex_search(tvalues, match, regex)) 
                {
                    currentsvg->scale[0] = std::stof(match[1].str());
                    currentsvg->scale[1] = std::stof(match[2].str());
                    currentsvg->rotate   = std::stof(match[3].str());
                    currentsvg->translate[0] = std::stof(match[4].str());
                    currentsvg->translate[1] = std::stof(match[5].str());                    
                    std::cerr << "scale: " << currentsvg->scale[0] << ":"<< currentsvg->scale[1];
                    std::cerr << " rotate: " << currentsvg->rotate;
                    std::cerr << " translate: [" << currentsvg->translate[0] << "," << currentsvg->translate[1] << "]" << std::endl;
                    r = r + 1;
                }
            }        
            else
            {      
                /* node example 
                <!-- cm_lan -->
                <g id="node1" class="node">
                <title>cm_lan</title>
                <polygon fill="none" stroke="black" points="281.69,-260.3 281.69,-309.9 361.69,-309.9 361.69,-260.3 281.69,-260.3"/>
                <text text-anchor="middle" x="321.69" y="-292.6" font-family="Times,serif" font-size="14.00">cm_lan.cpp</text>
                <polyline fill="none" stroke="black" points="281.69,-285.1 361.69,-285.1"/>
                <text text-anchor="middle" x="321.69" y="-267.8" font-family="Times,serif" font-size="14.00">cm_lan.h</text>
                </g>            
                */              
                if (reader.attributes().value("class").toString().toStdString()=="node")
                {       
                    xmlnode newn;
                    newn.id = reader.attributes().value("id").toString().toStdString();

                    reader.readNext();                    
                    while (reader.name().toString().toStdString() != "title")
                        reader.readNext();
                    newn.title = reader.readElementText().toStdString();                    
                    while (reader.name() != "polygon") 
                        reader.readNext();                
                    QString pointstring = reader.attributes().value("points").toString();                    
                    QStringList lpoints = pointstring.split(' ');
                    /* COMPUTE CENTROID */
                    qreal sumX = 0.0;
                    qreal sumY = 0.0;
                    for (const QString& point : lpoints)
                    {                        
                        QStringList coords = point.split(',');
                        if (coords.size() == 2)
                        {
                            qreal x = coords[0].toFloat();
                            qreal y = coords[1].toFloat();
                            sumX += x;
                            sumY += y;
                        }
                    }
                    qreal centroidX = sumX / lpoints.size();
                    qreal centroidY = sumY / lpoints.size();
                    newn.center = QPointF(centroidX,centroidY);
                    /* STORE BOUNDING POINTS */                    
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
                    reader.readNext();                    
                    while (reader.name() != "text")
                        reader.readNext();                    
                    newn.label = reader.readElementText().toStdString();                    
                    
                    currentsvg->nodes.push_back(newn);
                    //std::cerr << newn.id << " " << newn.title << " " << newn.label << std::endl;
                    //std::cerr << "CENTER [" << newn.center.x() << "," << newn.center.y() << "]" << std::endl;
                    for (auto each : newn.bounding)
                    {
                        //std::cerr << "\t" << each.x() << " " << each.y() << std::endl;
                    }
                    r = r + 1;
                }                
            }
        }        
    }    
    debugqt("qcanvas::xmlingest: finish rendering " + std::to_string(r));
    int nodes_number = currentsvg->nodes.size();
    debugqt("qcanvas::xmlingest: nodes rendered " + std::to_string(nodes_number));    
    return(nodes_number);
}

