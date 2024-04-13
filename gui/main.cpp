/* external standard libraries */
#include <iostream>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <qglobal.h>

/* graphviz */
#include <graphviz_version.h>

/* project GUI headers */
#include <cm_qt5_gui.h>
#include <gitinfo.h>

using namespace std;

int main(int argc, char** argv)
{ 
    /* codemapper GUI */
    QApplication cmgui(argc, argv);
    cmgui.setWindowIcon(QIcon(ICONPATH));

    cerr << "codemapper by " << CODEMAPPER_AUTHOR << " (C) 2024 " << GIT_OFFICIAL_VERSION << endl;                              
    cerr << argv[0] << " is GUI from codemapper tools, for more info, you're wellcome to visit: " << CODEMAPPER_URL << endl;
    cerr << "this release was tagged as " << GIT_OFFICIAL_VERSION << " version number" << endl;
    cerr << "Qt " << QT_VERSION_STR << " is included." << endl;
    cerr << "graphviz rendering has been included from version " << PACKAGE_VERSION << endl;

    auto win = new cm_qt5_gui;
    win->show();

    return cmgui.exec();
}