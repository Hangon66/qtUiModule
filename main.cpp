#include "uipushbuttnwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    uiPushButtnWidget w;
    w.show();
    return a.exec();
}
