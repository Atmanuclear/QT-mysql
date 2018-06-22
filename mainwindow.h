#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlQueryModel>
#include <QSqlQuery>

#include <QDialog>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();





private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_5_clicked();

    void on_pushButton_6_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_8_clicked();

    void on_pushButton_4_clicked();

    void on_pushButton_3_clicked();

    void on_pushButton_9_clicked();

    //void on_pushButton_10_clicked();

    //void on_pushButton_11_clicked();

    void on_pushButton_12_clicked();

    //void on_spinBox_valueChanged(int arg1);

    //void on_comboBox_currentIndexChanged(int index);

    void on_pushButton_13_clicked();

    void on_pushButton_14_clicked();

private:
    Ui::MainWindow *ui;
    QSqlTableModel *model;
    QSqlTableModel *model2;
    QSqlTableModel *model3;
    int skin;
    int sname;
    int week;
    int color;

};


#endif // MAINWINDOW_H
