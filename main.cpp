#include <QtGui/QApplication>
#include "tweezers.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Tweezers w;
    w.show();
    return a.exec();
}
