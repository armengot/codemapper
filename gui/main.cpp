/* external standard libraries */
#include <iostream>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <qglobal.h>

/* project GUI class */
#include <cm_qt5_gui.h>
#include <gitinfo.h>

using namespace std;

int main(int argc, char** argv)
{ 
    QApplication app(argc, argv);

    cerr << "codemapper by Marcelo Armengot (C) 2024 " << GIT_OFFICIAL_VERSION << endl;                              
    cerr << argv[0] << " is GUI from codemapper tools, for more info, you're wellcome to visit https://github.com/armengot/codemapper" << endl;
    cerr << "this release was tagged as " << GIT_OFFICIAL_VERSION << " version number" << endl;
    cerr << "Qt " << QT_VERSION_STR << " is included." << endl;

    auto win = new cm_qt5_gui;
    win->show();

    return app.exec();
}