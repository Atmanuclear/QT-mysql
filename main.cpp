#include "mainwindow.h"
#include <QApplication>
#include "connection.h"
#include <QFile>
#include <QApplication>
#include <QMessageBox>

/*lass CommonHelper
{
public:
    static void setStyle(const QString &style) {
        QFile qss(style);
        qss.open(QFile::ReadOnly);
        qApp->setStyleSheet(qss.readAll());
        qss.close();
    }
};
*/
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
   QFile file(":/black.qss");
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

    // 加载QSS样式

    //CommonHelper::setStyle("white.qss");
    if(!createConnection())
        return 1;



    MainWindow w;
    w.show();

    return a.exec();
}
