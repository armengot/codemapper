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

/* graphviz */
#include <graphviz_version.h>

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
        string output;
        debugqt("graphviz call to svg build... ");
        cm_render(current_project->to_string(), output, CM_OUTPUT_SVG);        
        svg = output;  
        canvas->load(svg);  
    }
}

void cm_qt5_gui::guiabout()
{
    // debugqt("about");
    QMessageBox box;
    QString about;
    about = QString("codemapper by Marcelo Armengot (C) 2024 - ") + QString(GIT_OFFICIAL_VERSION) + QString("\n");
    about = about + QString("This application is a GUI for codemapper tools.\n");
    about = about + QString("For more information, visit: https://github.com/armengot/codemapper\n");
    about = about + QString("(+) Qt ") + QString(QT_VERSION_STR) + QString(" is included.\n");
    about = about + QString("(+) Graphviz ") + QString(PACKAGE_VERSION) + QString(" is used.\n");    
    box.setInformativeText(about);
    /*
    about += "codemapper by Marcelo Armengot (C) 2024 " + QString(GIT_OFFICIAL_VERSION);
    about += "\nThis application is a GUI for codemapper tools.\n";
    about += "For more information, visit: https://github.com/armengot/codemapper\n";
    about += "This release was tagged as " + QString(GIT_OFFICIAL_VERSION) + " version number.\n";
    about += "Qt " + QString(QT_VERSION_STR) + " is included.";    
    */
    //QMessageBox::information(this, "About", about, QMessageBox::Ok);
    /*
    QMessageBox box(QMessageBox::Information, "About", about, QMessageBox::Ok, this);    
    box.setTextFormat(Qt::RichText);    
    box.setFixedSize(200, 900);   
    box.exec();
    */

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