#include "mainwindow.h"


#include <QApplication>
#include <QTableView>
#include <QStandardItemModel>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w ;

    QCoreApplication::setApplicationName(QString("Supplier Manager"));
    w.setWindowTitle( QCoreApplication::applicationName());
    w.show();


    return a.exec();
}
