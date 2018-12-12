#include "mainwindow.h"
#include <QApplication>
#include <QGraphicsPixmapItem>
#include <fstream>
#include <QGraphicsScene>
#include <QGraphicsView>


int main(int argc, char *argv[]){


    QApplication a(argc, argv);
    MainWindow w;

    w.show();





    return a.exec();
}
