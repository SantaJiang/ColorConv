#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFont font;
    font.setFamily("Arial Rounded MT Bold");
#ifdef Q_OS_WIN
    font.setPixelSize(10);
#else
    font.setPixelSize(12);
#endif

    a.setFont(font);
    MainWindow w;
    w.show();
    return a.exec();
}
