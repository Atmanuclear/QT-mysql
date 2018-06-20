#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new QSqlTableModel(this);
    model2 = new QSqlTableModel(this);
    model3 = new QSqlTableModel(this);
    model->setTable("student");
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select(); //选取整个表的所有行
    skin = 0;

    //不显示name属性列,如果这时添加记录，则该属性的值添加不上
    // model->removeColumn(1);

   // ui->tableView->setModel(model);

    //使其不可编辑
    //ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    model->database().transaction(); //开始事务操作
    if (model->submitAll()) {
        model->database().commit(); //提交
    } else {
        model->database().rollback(); //回滚
        QMessageBox::warning(this, tr("tableModel"),
                             tr("数据库错误: %1")
                             .arg(model->lastError().text()));
    }
}


void MainWindow::on_pushButton_2_clicked()
{
    model->revertAll();
}

void MainWindow::on_pushButton_5_clicked()
{
    QString name = ui->lineEdit->text();
    //根据姓名进行筛选
    model->setFilter(QString("name = '%1'").arg(name));
    //显示结果
    model->select();
}

void MainWindow::on_pushButton_6_clicked()
{
    model->setTable("appointment");   //重新关联表
    model->select();
    ui->tableView->setModel(model);
    //这样才能再次显示整个表的内容
}

// 升序
void MainWindow::on_pushButton_7_clicked()
{
    //id属性即第0列，升序排列
    model->setSort(0, Qt::AscendingOrder);
    model->select();
}
// 降序
void MainWindow::on_pushButton_8_clicked()
{
    model->setSort(0, Qt::DescendingOrder);
    model->select();
}

void MainWindow::on_pushButton_4_clicked()
{
    //获取选中的行
    int curRow = ui->tableView->currentIndex().row();

    //删除该行
    model->removeRow(curRow);

    int ok = QMessageBox::warning(this,tr("删除当前行!"),
                                  tr("你确定删除当前行吗？"),
                                  QMessageBox::Yes,QMessageBox::No);
    if(ok == QMessageBox::No)
    {
        model->revertAll(); //如果不删除，则撤销
    }
    else model->submitAll(); //否则提交，在数据库中删除该行
}

void MainWindow::on_pushButton_3_clicked()
{
    int rowNum = model->rowCount(); //获得表的行数
    int id = 10;
    model->insertRow(rowNum); //添加一行
    model->setData(model->index(rowNum,0),id);
    //model->submitAll(); //可以直接提交
}

void MainWindow::on_pushButton_9_clicked()
{

   /* model2->setTable("test");   //重新关联表
    model2->select();
    ui->tableView_2->setModel(model2);
    */
    QSqlQuery query;
    sname=(ui->comboBox_2->currentIndex());
    ui->tableView_2->setEditTriggers(QAbstractItemView::NoEditTriggers);

    switch (sname) {
    case 0:
        model2->setTable("teachers");   //重新关联表
        model2->select();
        ui->tableView_2->setModel(model2);
        break;
    case 1:
        model2->setTable("courses");   //重新关联表
        model2->select();
        ui->tableView_2->setModel(model2);
        break;
    case  2:
        model2->setTable("rooms");   //重新关联表
        model2->select();
        ui->tableView_2->setModel(model2);
        break;
    case  3:
        model2->setTable("plan");   //重新关联表
        model2->select();
        ui->tableView_2->setModel(model2);
        break;
    case  4:
        model2->setTable("appointment");   //重新关联表
        model2->select();
        ui->tableView_2->setModel(model2);
        break;

    default:
        break;
    }

}

void MainWindow::on_pushButton_10_clicked()
{
    /*QSqlQuery query = model3->query();

    query.exec("insert name from teachers where tcid = 123123");
    QString tname = ui->lineEdit_2->text();

    model3->setQuery("select tname from teachers where tcid = 123123 ");

    ui->tableView_2->setModel(model3);
    */
   QString qss;
    if(skin == 0){
        skin = 1;
        qss = ":/orange.qss";
    }
    else{
        skin =0 ;
         qss = ":/black.qss";
    }
    QFile file(qss);

    file.open(QFile::ReadOnly);

    if(file.isOpen())
    {
        QString btnstylesheet = QObject::tr(file.readAll());
        qApp->setStyleSheet(btnstylesheet);
        file.close();

    }

}


void MainWindow::on_pushButton_11_clicked()
{

}


void MainWindow::on_pushButton_12_clicked()
{
    QSqlQuery query;
    query.exec("select * from view student_apt");
    if(query.size())
        query.exec("drop view student_apt");
    QString csid = ui->lineEdit_2->text();
    week=(ui->spinBox->value());
    QString s;
    s= QString::number(week, 10);
    query.exec("create view student_apt(aptday,aptperiod,crsid,rmid) as select appointment.aptday,aptperiod,appointment.crsid,rmid from plan,appointment where aptweek = "+s+" and plan.crsid = appointment.crsid  and  classid = "+csid);
    model3->setTable("student_apt");   //重新关联表
    model3->select();
    ui->tableView_3->setModel(model3);
    qDebug() << "successed" << query.lastError();

}

