/* standar external */
#include <iostream>
#include <thread>

/* Qt5 libs */
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QMenuBar>
#include <QMenu>
#include <QCloseEvent>
#include <QMessageBox>
#include <QLayout>
#include <QObject>
#include <QFileDialog>
#include <QTextStream>
#include <QMouseEvent>

/* graphviz */
#include <gvc.h>
#include <graphviz_version.h>

/* PROJECT LIBS */
#include <cm_qt5_gui.h>
#include <cm_clan.h>
#include <cm_pylan.h>
#include <tools.h>

using namespace std;

/* constructor */
cm_qt5_gui::cm_qt5_gui()
{
    setWindowTitle(QCoreApplication::applicationName());
    resize(500,300);

    debugqt("Generic C++/Qt5 setup...");
    debugqt("GNU/Linux or whatever platform (no Win32) detected.");

    debugqt("setup GUI ...");

    setup_canvas();
    setup_menus();   
}

void cm_qt5_gui::keyReleaseEvent(QKeyEvent *event)
{
    std::cerr << "cm_qt5_gui::keyReleaseEvent : " << canvas->selected_node << std::endl;
    if (event->key() == Qt::Key_Tab)
    {        
        if (canvas->selected_node != "")
        {
            /* std::cerr << "tab" << std::endl; */
            QPoint pos;
            QMouseEvent *fake = new QMouseEvent(QEvent::MouseButtonPress, pos, Qt::RightButton, Qt::RightButton, Qt::NoModifier);
            debugqt("sending right mouse event to canvas");
            canvas->callable_rightmouse(fake);
        }  
        else
        {
            if (canvas->selected_edge != nullptr)
            {
                cm_edge* next = current_project->nextedge(canvas->selected_edge);
                canvas->select_edge(next);
            }
        }      
    }
}

void cm_qt5_gui::keyPressEvent(QKeyEvent *event)
{    
    QKeyEvent* key = static_cast<QKeyEvent*>(event);
    std::cerr << "KEY PRESSED" << std::endl;
    if (key->key() == Qt::Key_Escape) 
    {
        quitcool();
    }
    else if (key->key() == Qt::Key_Delete)
    {
        canvas->deletenode();
        canvas->deleteedge();
    }
    else
    {
        debugqt(QKeySequence(key->key()).toString().toStdString());
    }
}

void cm_qt5_gui::setup_canvas()
{
    sarea = new QScrollArea;
    sarea->setBackgroundRole(QPalette::Dark);
    sarea->setWidgetResizable(true);

    /* text bar */
    textbar = new QTextEdit;
    textbar->setReadOnly(true);
    textbar->setFixedHeight(40);

    /* create canvas */
    canvas = new qcanvas();
    //canvas->setParent(sarea);
    canvas->setMouseTracking(true);
    canvas->setBackgroundRole(QPalette::Base);
    canvas->setScaledContents(true);

    /* Scroll Area -> canvas*/
    sarea->setWidget(canvas);

    /* layout -> canvas+text */
    QVBoxLayout* layout = new QVBoxLayout;
    layout->addWidget(sarea);
    layout->addWidget(textbar); 

    /* output link */
    canvas->canvas_textline = textbar;

    /* container */
    QWidget* container = new QWidget;
    container->setLayout(layout);    

    /* main widget -> scroll area */
    // setCentralWidget(sarea);
    setCentralWidget(container);
}


void cm_qt5_gui::setup_menus()
{
    /* init menu bar */
    QMenuBar *mbar = menuBar();
    QMenuBar *rbar = new QMenuBar(mbar);
    mbar->setCornerWidget(rbar);

    /* sub menus */
    /* --------- */
    /* FILE */
    QMenu *file = mbar->addMenu("&File");
    QMenu *open = file->addMenu("&Open");
    /* LANGUAGE */
    QMenu *lan_menu = mbar->addMenu("&Language");    

    /* HELP */
    QMenu *help = rbar->addMenu("&Help");

    /* actions */
    /* ------- */
    /* FILE MENU */
    QAction *openfolder = open->addAction(tr("&Folder"), this, &cm_qt5_gui::infolder);
    QAction *openfile = open->addAction(tr("&Project file"), this, &cm_qt5_gui::infile);
    file->addSeparator();
    /* ---------------- */    
    QAction *save = file->addAction(tr("&Save"), this, &cm_qt5_gui::project_save);
    save->setShortcut(Qt::Key_S);
    file->addAction(save);
    file->addSeparator();
    /* ---------------- */        
    QAction *quit = file->addAction(tr("&Quit"), this, &cm_qt5_gui::quitcool);
    quit->setShortcut(Qt::Key_Q);
    file->addAction(quit);


    /* LANGUAGE MENU -> SWITCH among lan options */
    /* ----------------------------------------- */        
    QActionGroup *lanswitch = new QActionGroup(this);
    lanswitch->setExclusive(true);
    /* --------------------------------------------------------------------------------------------------- */
    QAction *select_c = new QAction(QString::fromStdString("C (lan)"), this);
    select_c->setCheckable(true);
    select_c->setChecked(false);
    QObject::connect(select_c, &QAction::triggered, this, [this]() { this->select_language("clan"); });
    lanswitch->addAction(select_c);
    /* --------------------------------------------------------------------------------------------------- */
    QAction *select_cpp = new QAction(QString::fromStdString("C++"), this);
    select_cpp->setCheckable(true);
    select_cpp->setChecked(true);
    QObject::connect(select_cpp, &QAction::triggered, this, [this]() { this->select_language("cpp"); });
    lanswitch->addAction(select_cpp);
    /* --------------------------------------------------------------------------------------------------- */
    QAction *select_py = new QAction(QString::fromStdString("Python"), this);
    select_py->setCheckable(true);
    select_py->setChecked(false);
    QObject::connect(select_py, &QAction::triggered, this, [this]() { this->select_language("py"); });
    lanswitch->addAction(select_py);
    /* --------------------------------------------------------------------------------------------------- */    
    lan_menu->addAction(select_c);
    lan_menu->addAction(select_cpp);
    lan_menu->addAction(select_py);

    /* HELP MENU */
    QAction *about = help->addAction(tr("&About"),this,&cm_qt5_gui::guiabout);

    /* linking */    
    //connect(quit,  &QAction::triggered, qApp, &cm_qt5_gui::quitcool);

}

void cm_qt5_gui::select_language(std::string lan)
{
    debugqt("Selecting language: "+lan);
    lan_selected = true;
    if (lan == "cpp")    
    {
        selected_input_language = "cpp";
    }
    else if (lan == "py")
    {
        selected_input_language = "py";
    }
    else if (lan == "clan")
    {
        selected_input_language = "c  ";
    }
}

void cm_qt5_gui::project_save()
{    
    if (current_project==nullptr)
    {
        QMessageBox warning;
        warning.setWindowTitle("Warning");
        warning.setText("There is no project loaded currently.");
        warning.setStandardButtons(QMessageBox::Ok);    
        warning.exec();        
    }
    else
    {
        QString starter = QDir::currentPath();    
        QString target = QFileDialog::getSaveFileName(this, tr("Save Project"), starter, tr("SVG Files (*.svg);;DOT Files (*.dot);;PNG Files (*.png)"));

        if (!target.isEmpty()) 
        {
            QFileInfo target_info(target);
            QString extension = target_info.suffix();
            debugqt("Saving project to file: " + target.toStdString());
            string foutput, output = current_project->to_string();                
            cm_dashclean(output);                
            debugqt("graphviz call to svg build... ");
            if (extension.toLower() == "svg") 
            {
                cm_render(output, foutput, CM_OUTPUT_SVG);        
            }
            else if (extension.toLower() == "png") 
            {
                cm_render(output, foutput, CM_OUTPUT_PNG);
            }
            else
            {
                foutput = output;
            }
            QFile file(target);
            if (file.open(QIODevice::WriteOnly)) 
            {
                if (extension.toLower() == "png")
                {
                    file.write(foutput.data(), foutput.size());
                }
                else
                {
                    QTextStream fout(&file);
                    fout << QString::fromStdString(foutput);
                    file.close();
                    debugqt("Project saved successfully: "+target.toStdString());
                }
            } 
            else 
            {
                debugqt("Unable to open file for writing.");
            }        
        }
    }
}


void cm_qt5_gui::quitcool()
{
    QMessageBox sure;
    sure.setWindowTitle("Quit");
    sure.setText("Exit of "+QCoreApplication::applicationName()+"\nAre you sure?");
    sure.setStandardButtons(QMessageBox::Ok);
    sure.addButton(QMessageBox::Cancel);
    sure.setDefaultButton(QMessageBox::Cancel);

    if(sure.exec() == QMessageBox::Ok)
        QApplication::quit();
}

void cm_qt5_gui::infile()
{    
    QString starter = QDir::currentPath();    
    QString target = QFileDialog::getOpenFileName(this, tr("Open file"), starter, tr("DOT Files (*.dot)"));

    if (!target.isEmpty()) 
    {
        /* graphviz dot reader link */
        GVC_t *gvc;
        Agraph_t *g;
        FILE *fp;
        gvc = gvContext();
        fp = fopen(target.toStdString().c_str(), "r");        
        g = agread(fp, 0);  
        if (current_project!=nullptr)      
        {
            delete(current_project);
        }
        if (g == NULL)
        {
            QMessageBox warning;
            warning.setWindowTitle("Warning");
            warning.setText("Invalid file.");
            warning.setStandardButtons(QMessageBox::Ok);    
            warning.exec();
            return;
        }
        current_project = new cm_graph(g);
        gvFreeLayout(gvc, g);
        agclose(g);
        gvFreeContext(gvc);
        /* codemapper tricks and traps */
        /* to force reingest the xml */
        if (canvas->svg_loaded_as_xml)
        {
            canvas->svg_loaded_as_xml = false;
        }            
        string svg_output, output = current_project->to_string();                
        cm_dashclean(output);                        
        int r = cm_render(output, svg_output, CM_OUTPUT_SVG);        
        svg = svg_output;                
        canvas->load(svg);
        canvas->setgraph(current_project);
    }
}


void cm_qt5_gui::infolder()
{   
    if (!lan_selected)
    {
        QMessageBox warning;
        warning.setWindowTitle("Warning");
        warning.setText("Please select first the source code language in Language menu.");
        warning.setStandardButtons(QMessageBox::Ok);    
        warning.exec();
    }
    else
    {
        QString folder = QFileDialog::getExistingDirectory(0, ("Select folder"), QDir::currentPath());
        if (!folder.isEmpty())
        {
            /* to force reingest the xml */
            if (canvas->svg_loaded_as_xml)
            {
                canvas->svg_loaded_as_xml = false;
            }  
            if (current_project != nullptr)
            {
                delete(current_project);
            }
            string folder_input = folder.toStdString();
            debugqt("Folder selected: "+folder_input);
            if (selected_input_language[0]=='c')
            {
                debugqt("Parsing "+folder_input+" C/C++ project");
                cpp_language input_source_code(folder_input);
                if (selected_input_language == "cpp")
                    input_source_code.mode_c_or_cpp(1); // C++
                else
                    input_source_code.mode_c_or_cpp(0); // C
                current_project = input_source_code.parse(); /* implicit new */
                input_source_code.create_nodes(current_project);
                input_source_code.create_edges(current_project);        
                debugqt("CODEMAPPER graph created, rendering ... ");
                string svg_output, output = current_project->to_string();                
                cm_dashclean(output);                
                debugqt("graphviz call to svg build... ");
                int r = cm_render(output, svg_output, CM_OUTPUT_SVG);
                if (r!=0)
                {
                    QMessageBox error;
                    error.setWindowTitle("Graphviz error");
                    error.setText("Errors happened while Graphviz rendering, check about keywords in source (graph, node, edge) or reserver chars (-).");
                    error.setStandardButtons(QMessageBox::Ok);    
                    error.exec();
                }
                else
                {                        
                    svg = svg_output;                
                    canvas->load(svg);
                    canvas->setgraph(current_project);                
                }
            }
            else if (selected_input_language == "py")
            {
                debugqt("Parsing "+folder_input+" Python project");
                py_language input_source_code(folder_input);
                current_project = input_source_code.parse();
                input_source_code.create_nodes(current_project);
                input_source_code.create_edges(current_project);
                debugqt("CODEMAPPER graph created, rendering ... ");
                string svg_output, output = current_project->to_string();
                cm_dashclean(output);
                debugqt("graphviz call to svg build... ");
                int r = cm_render(output, svg_output, CM_OUTPUT_SVG);        
                if (r!=0)
                {
                    QMessageBox error;
                    error.setWindowTitle("Graphviz error");
                    error.setText("Errors happened while graphviz rendering, check about keywords in source (graph, node, edge) or reserver chars (-).");
                    error.setStandardButtons(QMessageBox::Ok);    
                    error.exec();
                }
                else
                {                        
                    svg = svg_output;                
                    canvas->load(svg);
                    canvas->setgraph(current_project);                
                }
            }            
        }
    }
}

void cm_qt5_gui::guiabout()
{
    QMessageBox box;
    QString about;
    about = QString("codemapper by Marcelo Armengot (C) 2024 - ") + QString(GIT_OFFICIAL_VERSION) + QString("\n");
    about = about + QString("This application is a GUI for codemapper tools.\n");
    about = about + QString("For more information, visit: ") + QString(CODEMAPPER_URL) + QString("\n");
    about = about + QString("(+) Qt ") + QString(QT_VERSION_STR) + QString(" is included.\n");
    about = about + QString("(+) Graphviz ") + QString(PACKAGE_VERSION) + QString(" is used.\n");    
    box.setInformativeText(about);
    QSpacerItem* horizontal = new QSpacerItem(800, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    QGridLayout* layout = (QGridLayout*)box.layout();
    layout->addItem(horizontal, layout->rowCount(), 0, 1, layout->columnCount());
    box.exec();
}

QString cm_qt5_gui::getversion()
{
    return QString(GIT_OFFICIAL_VERSION);
}

void cm_qt5_gui::canvaslink(QPoint mouse)
{  
    std::string tmp = "FATHER = [" + std::to_string(mouse.x()) +"," +std::to_string(mouse.y()) + "]";    
    debugqt(tmp);
}

void cm_qt5_gui::closeEvent(QCloseEvent *event)
{
    event->ignore();
    quitcool();
}