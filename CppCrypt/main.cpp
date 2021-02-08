#include "CppCrypt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CppCrypt w;
    QApplication::setStyle("fusion");
    w.adjustSize();
    w.setFixedSize(w.size());
    w.show();
    return a.exec();
}
