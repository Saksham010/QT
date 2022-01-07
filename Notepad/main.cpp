#include "mainwindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication::setStyle("fusion");

    QApplication a(argc, argv);

    a.setStyleSheet(QStringLiteral("QMessageBox{"
                                                "border-radius:30px;"
                                                                    "}"));
    MainWindow w;
    w.show();
    return a.exec();
}
