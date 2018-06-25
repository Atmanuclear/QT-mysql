#include "mainwindow.h"
#include <QApplication>
#include "connection.h"
#include "tipsdialog.h"
#include <QMessageBox>

// 加载QSS样式
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile file(":/black.qss");
    if( file.open(QFile::ReadOnly)){
        QString styleSheet = QLatin1String(file.readAll());
        a.setStyleSheet(styleSheet);
        file.close();
    }
    else{
        QMessageBox::warning(NULL, "warning", "Open failed", QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
    }
    if(!createConnection()) return 1;

    TipsDialog dlg;
    MainWindow w;
    dlg.setWindowTitle("登录");
    w.setWindowTitle("HIT教务排课系统");

    /*
    if(dlg.exec() == QDialog::Accepted) // 利用Accepted返回值判断按钮是否被按下
    {
        w.show();                       // 如果被按下，显示主窗口
        return a.exec();               // 程序一直执行，直到主窗口关闭
    }
    else return 0;
    */
    w.show();
    return a.exec();
}
