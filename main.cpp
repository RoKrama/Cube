#include "cube.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    cube w;
    w.show();
    return a.exec();
}
