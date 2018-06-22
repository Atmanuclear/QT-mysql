#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QDialog>
#include <QMessageBox>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    model = new QSqlTableModel(this);
    model2 = new QSqlTableModel(this);
    model3 = new QSqlTableModel(this);
    model->setTable("appointment");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select(); //选取整个表的所有行
    skin = 0;

    ui->tableView->setModel(model);

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
    }
    else {
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



void MainWindow::on_pushButton_13_clicked()
{
    QString qss;
    color=(ui->comboBox->currentIndex());
    switch (color) {
    case 0:
        qss = ":/black.qss";
        break;
    case 1:
        qss = ":/style.qss";
        break;
    case 2:
        qss = ":/orange.qss";
        break;
    default:
        break;
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

void MainWindow::on_pushButton_14_clicked()
{
    QSqlQuery query;
    query.exec("select * from view teachers_apt");
    if(query.size())
        query.exec("drop view teachers_apt");
    QString tcid = ui->lineEdit_3->text();
    week=(ui->spinBox_2->value());
    QString s;
    s= QString::number(week, 10);
    query.exec("create view teachers_apt(aptday,aptperiod,crsid,rmid) as select appointment.aptday,aptperiod,appointment.crsid,rmid from courses,appointment where aptweek = "+s+" and courses.crsid = appointment.crsid  and  tcid = "+tcid);
    model3->setTable("teachers_apt");   //重新关联表
    model3->select();
    ui->tableView_4->setModel(model3);
    qDebug() << "successed" << query.lastError();
}

void MainWindow::on_pushButton_11_clicked()
{
    QSqlQueryModel *qmodel = new QSqlQueryModel();

    qmodel->setQuery("SELECT * FROM teachers");
    for(int i = 0; i < qmodel->rowCount(); i++){
        Teacher t;

        t.tchId = qmodel->record(i).value(0).toString().toStdString();
        teachers.push_back(t);
    }

    qmodel->setQuery("SELECT * FROM courses");
    for(int i = 0; i < qmodel->rowCount(); i++){
        Course c;
        int wmin, wmax;
        std::string tch;

        c.crsId = qmodel->record(i).value(0).toString().toStdString();
        c.crsName = qmodel->record(i).value(1).toString().toStdString();
        tch = qmodel->record(i).value(2).toString().toStdString();
        for(int j=0; j<teachers.size(); j++)
            if(teachers[j].tchId == tch){
                c.tutor = j;
                break;
            }
        wmin = qmodel->record(i).value(3).toInt();
        wmax = qmodel->record(i).value(4).toInt();
        for(int i=wmin; i<=wmax; i++) c.tchWeek[i-1] = true;
        c.weekcnt = qmodel->record(i).value(5).toInt();
        c.env = qmodel->record(i).value(6).toInt();
        c.inserted = qmodel->record(i).value(1).toInt();
        courses.push_back(c);
    }

    qmodel->setQuery("SELECT * FROM rooms");
    for(int i = 0; i < qmodel->rowCount(); i++){
        Site s;
        s.siteId = qmodel->record(i).value(0).toString().toStdString();
        s.capacity = qmodel->record(i).value(1).toInt();
        s.type = qmodel->record(i).value(2).toInt();
        sites.push_back(s);
    }

    qmodel->setQuery("SELECT * FROM plan");
    for(int i = 0; i < qmodel->rowCount(); i++){
        std::string crsid = qmodel->record(i).value(0).toString().toStdString();
        std::string clsid = qmodel->record(i).value(1).toString().toStdString();

        bool classExist = false;
        int clsindex;
        for(int j = 0; j < classes.size(); j++){
            if(classes[j].classId == clsid){
                classExist = true;
                clsindex = j;
                break;
            }
        }

        for(int j = 0; j < courses.size(); j++){
            if(courses[j].crsId == crsid){
                if(!classExist){
                    classes.push_back(StuClass(clsid));
                    clsindex = classes.size()-1;
                }
                courses[j].attCls.push_back(clsindex);
                break;
            }
        }
    }

    qmodel->setQuery("SELECT * FROM appointment");
    for(int i = 0; i < qmodel->rowCount(); i++){
        Appointment app;
        app.week = qmodel->record(i).value(0).toInt();
        app.day = qmodel->record(i).value(1).toInt();
        app.period = qmodel->record(i).value(2).toInt();

        std::string crsid = qmodel->record(i).value(3).toString().toStdString();
        std::string rmid = qmodel->record(i).value(4).toString().toStdString();

        for(int j = 0; j < courses.size(); j++)
            if(courses[j].crsId == crsid){
                app.course = j;
                break;
            }

        for(int j = 0; j < sites.size(); j++)
            if(sites[j].siteId == rmid){
                app.room = j;
                break;
            }
    }
    qDebug() << teachers.size();
    qDebug() << classes.size();
    qDebug() << courses.size();
    qDebug() << sites.size();
    qDebug() << appms.size();

    arranging();
    QSqlQuery *query = new QSqlQuery();
    qDebug() << appms.size();
    query->exec(QString("DELETE FROM appointment"));

    QSqlTableModel *tmodel = new QSqlTableModel(this);
    tmodel->setTable("appointment");
    tmodel->setEditStrategy(QSqlTableModel::OnFieldChange);
    int rowCount;
    for(int i = 0; i < appms.size(); i++){
        rowCount = tmodel->rowCount();
        tmodel->insertRow(rowCount);
        tmodel->setData(tmodel->index(rowCount,0),appms[i].week);
        tmodel->setData(tmodel->index(rowCount,1),appms[i].day);
        tmodel->setData(tmodel->index(rowCount,2),appms[i].period);
        tmodel->setData(tmodel->index(rowCount,3),QString::fromStdString(courses[appms[i].course].crsId));
        tmodel->setData(tmodel->index(rowCount,4),QString::fromStdString(sites[appms[i].room].siteId));
        tmodel->submit();
    }
    on_pushButton_6_clicked();
}
