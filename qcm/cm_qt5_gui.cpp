#include <iostream>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QMenuBar>
#include <QMenu>
#include <QCloseEvent>
#include <QMessageBox>
#include <QLayout>
#include <QObject>

/* PROJECT LIBS */
#include <cm_qt5_gui.h>

using namespace std;

/* constructor */
cm_qt5_gui::cm_qt5_gui()
{
    setWindowTitle(QCoreApplication::applicationDirPath());
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
    //sarea = new QScrollArea;

    canvas->setBackgroundRole(QPalette::Base);
    canvas->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Ignored);
    canvas->setScaledContents(true);
        
    //setCentralWidget(&canvas);    

    //sarea->setBackgroundRole(QPalette::Dark);
    //sarea->setWidget(canvas);
    //sarea->setVisible(false);
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
    // connect(quit,  &QAction::triggered, qApp, &cm_qt5_gui::quitcool);

}

void cm_qt5_gui::quitcool()
{
    QMessageBox sure;
    sure.setWindowTitle("Quit");
    sure.setText("Are you sure?");
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
    debugqt("in folder");
}

void cm_qt5_gui::guiabout()
{
    debugqt("about");
}