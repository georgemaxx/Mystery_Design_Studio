#include "mainwindow.h"

#include <QApplication>

MainWindow *w;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    w = new MainWindow;
    w->setWindowTitle("Mystery Design Studio");
    w->setWindowIcon(QIcon(":/icons/icons/app_icon.svg"));
    w->show();
    return a.exec();
}
