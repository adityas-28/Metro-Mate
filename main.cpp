#include "mainwindow.h"

#include <QApplication>
// #include <iostream>

int main(int argc, char *argv[])
{
 //   std::cout << "before mainwindow";
    QApplication a(argc, argv);
    a.setStyleSheet("QWidget { background-color: black; color: white; }");
    MainWindow w;
    w.show();
    return a.exec();
}
