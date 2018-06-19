#include "mainwindow.h"
#include <QApplication>
#include "connection.h"
#include <QMessageBox>

// 加载QSS样式
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file(":/style.qss");
       if( file.open(QFile::ReadOnly))
       {
           QString styleSheet = QLatin1String(file.readAll());

           a.setStyleSheet(styleSheet);
           file.close();

       }
       else
       {

           QMessageBox::warning(NULL, "warning", "Open failed", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);

       }


    if(!createConnection())
        return 1;



    MainWindow w;
    w.show();

    return a.exec();
}
