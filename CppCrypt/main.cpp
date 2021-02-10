#include "CppCrypt.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CppCrypt w;
    QApplication::setStyle("fusion");
    w.adjustSize();
    w.setFixedSize(w.size());

    QFile file("styles.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    a.setStyleSheet(styleSheet);

    w.show();
    return a.exec();
}
