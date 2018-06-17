#include "mainwindow.h"
#include <QApplication>
#include "connection.h"
#include <QFile>
#include <QApplication>


class CommonHelper
{
public:
    static void setStyle(const QString &style) {
        QFile qss(style);
        qss.open(QFile::ReadOnly);
        qApp->setStyleSheet(qss.readAll());
        qss.close();
    }
};

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    if(!createConnection())
        return 1;


    MainWindow w;
    w.show();

    return a.exec();
}
