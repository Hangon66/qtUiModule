#include "uiTestWidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    uiTestWidget w;
    w.show();
    return a.exec();
}
