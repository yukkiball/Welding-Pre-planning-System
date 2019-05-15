#include "widget.h"
#include "data.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Widget w;
    w.setWindowTitle(QObject::tr("焊接路径规划系统"));
    w.show();
    return a.exec();
}
