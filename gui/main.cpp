#include <iostream>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>

#include <cm_qt5_gui.h>

using namespace std;

int main(int argc, char** argv)
{ 
    QApplication app(argc, argv);

    auto win = new cm_qt5_gui;
    win->show();

    return app.exec();
}