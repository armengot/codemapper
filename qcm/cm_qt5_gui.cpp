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
#include <QByteArray>
#include <QSvgRenderer>
#include <QPainter>

/* PROJECT LIBS */
#include <cm_qt5_gui.h>
#include <cm_clan.h>
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

void cm_qt5_gui::debugqt(std::string stin)
{
    std::cerr << "debug(QT): " << stin << std::endl; 
}

void cm_qt5_gui::setup_canvas()
{
    canvas = new qcanvas();
    canvas->setParent(this);
    canvas->setMouseTracking(true);
    canvas->setBackgroundRole(QPalette::Base);
    //canvas->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    canvas->setScaledContents(true);
        
    //setCentralWidget(&canvas);    

    //sarea->setBackgroundRole(QPalette::Dark);
    setCentralWidget(canvas);
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

    /* VIEW */
    // QMenu *view = mbar->addMenu("&View");

    /* HELP */
    QMenu *help = rbar->addMenu("&Help");

    /* actions */
    /* ------- */
    /* FILE MENU */
    QAction *openfolder = open->addAction(tr("&Folder"), this, &cm_qt5_gui::infolder);
    QAction *openfile = open->addAction(tr("&Project file"), this, &cm_qt5_gui::infile);
    file->addSeparator();
    QAction *quit = file->addAction(tr("&Quit"), this, &cm_qt5_gui::quitcool);
    quit->setShortcut(Qt::Key_Q);
    file->addAction(quit);

    /* HELP MENU */
    QAction *about = help->addAction(tr("&About"),this,&cm_qt5_gui::guiabout);

    /* linking */    
    //connect(quit,  &QAction::triggered, qApp, &cm_qt5_gui::quitcool);

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
    debugqt("in file");
}

void cm_qt5_gui::infolder()
{    
    QString folder = QFileDialog::getExistingDirectory(0, ("Select folder"), QDir::currentPath());
    string folder_input = folder.toStdString();
    debugqt("Folder selected: "+folder_input);

    if (selected_input_language[0]=='c')
    {
        debugqt("Parsing as C/C++ project");
        cpp_language input_source_code(folder_input);
        current_project = input_source_code.parse();
        input_source_code.create_nodes(current_project);
        input_source_code.create_edges(current_project);        
        debugqt("CODEMAPPER graph created, rendering ... ");
        stringstream output;
        debugqt("graphviz call to svg build... ");
        cm_render(current_project->to_string(), output);        
        std::string svg = output.str();        
        QByteArray qbytes(svg.c_str(), static_cast<int>(svg.length()));
        QSvgRenderer renderer;
        renderer.load(qbytes);

        if (renderer.isValid()) 
        {
            QPixmap pixmap(renderer.defaultSize());
            pixmap.fill(Qt::transparent);
            QPainter painter(&pixmap);
            renderer.render(&painter);
            painter.end();    
            canvas->setPixmap(pixmap);
        }
        else 
        {
            debugqt("Error happened rendering SVG graph.");
        }
    }

}

void cm_qt5_gui::guiabout()
{
    debugqt("about");
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