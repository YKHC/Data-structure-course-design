#include "mainwindow.h"
#include "logon.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Logon w;
    w.show();
    return a.exec();
}
