#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QFile>
#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QAxObject>


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
    Excel();
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
    if(ok == QMessageBox::No){

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

void MainWindow::on_TimeTableBtn_clicked(){

    QSqlQuery query;
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(4);
    QStringList headers;
    headers<< "星期一"<< "星期二"<< "星期三"<< "星期四"<< "星期五"<<"**";
    QStringList vertical_header;
    /*vertical_header<<"第一节课"<<"第二节课"<<"第三节课"<<"第四节课";
    ui->tableWidget->setVerticalHeaderLabels(vertical_header);*/
    ui->tableWidget->setHorizontalHeaderLabels(headers);
    int day;
    int time;
    QString csid = ui->lineEdit_2->text();
    week=(ui->spinBox->value());
    QString s;
    s= QString::number(week, 10);
    query.exec("select aptweek,aptday,aptperiod,crsname,tname,rmid from plan,appointment,courses,teachers where aptweek = "+s+" and teachers.tcid = courses.tcid and courses.crsid = plan.crsid and plan.crsid = appointment.crsid  and  classid = "+csid);
    while(query.next()){
        qDebug() << "successed"<<query.value(3).toString()<<query.value(4).toString()<<query.value(5).toString()<< query.lastError();
        for(int i=1;i<6;i++){
            for(int j=1;j<5;j++){
                day = query.value(0).toInt();
                time = query.value(1).toInt();
                if(i==day && j==time){
                QString m=query.value(3).toString()+query.value(4).toString()+query.value(5).toString();
                ui->tableWidget->setItem(i-1, j-1,new QTableWidgetItem(m));
                }
            }
        }
    }

//        query.exec("select aptweek,aptday,aptperiod,crsname,tname,rmid from plan,appointment,courses,teachers where aptday = "+j+" and aptperiod = "+i+"and aptweek = "+s+" and teachers.tcid = courses.tcid and courses.crsid = plan.crsid and plan.crsid = appointment.crsid  and  classid = "+csid);
//        query.next();
//        qDebug() << "successed"<<query.value(3).toString()<<query.value(4).toString()<<query.value(5).toString()<< query.lastError();
//    ui->tableWidget->setItem(0, 0, new QTableWidgetItem(QString("0001")));
//    ui->tableWidget->setItem(1, 0, new QTableWidgetItem(QString("0002")));
//    ui->tableWidget->setItem(2, 0, new QTableWidgetItem(QString("0003")));
//    ui->tableWidget->setItem(3, 0, new QTableWidgetItem(QString("0004")));
//    ui->tableWidget->setItem(4, 0, new QTableWidgetItem(QString("0005")));
//    ui->tableWidget->setItem(0, 1, new QTableWidgetItem(QString("20100112")));

    ui->tableWidget->show();

}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    on_pushButton_12_clicked();
}

void MainWindow::Excel(){
    on_TimeTableBtn_clicked();
    int i,j;
    QAxObject* excel = new QAxObject(this);
    excel->setControl("Excel.Application");  // 连接Excel控件
    excel->dynamicCall("SetVisible (bool Visible)", "true"); // 不显示窗体
    excel->setProperty("DisplayAlerts", true);  // 不显示任何警告信息。如果为true, 那么关闭时会出现类似"文件已修改，是否保存"的提示

    // step2: 打开工作簿
    QAxObject* workbooks = excel->querySubObject("WorkBooks"); // 获取工作簿集合
    // 打开工作簿方式一：新建
    workbooks->dynamicCall("Add"); // 新建一个工作簿
    QAxObject* workbook = excel->querySubObject("ActiveWorkBook"); // 获取当前工作簿
    // 打开工作簿方式二：打开现成
    //QAxObject* workbook = workbooks->querySubObject("Open(const QString&)", ui->lineEdit_4->text()); // 从控件lineEdit获取文件名

    // step3: 打开sheet
    QAxObject* worksheet = workbook->querySubObject("WorkSheets(int)", 1); // 获取工作表集合的工作表1， 即sheet1


    // step4: 获取行数，列数
    QAxObject* usedrange = worksheet->querySubObject("UsedRange"); // sheet范围
    int intRowStart = usedrange->property("Row").toInt(); // 起始行数
    int intColStart = usedrange->property("Column").toInt();  // 起始列数

    QAxObject *rows, *columns;
    rows = usedrange->querySubObject("Rows");  // 行
    columns = usedrange->querySubObject("Columns");  // 列

    int intRow = rows->property("Count").toInt(); // 行数
    int intCol = columns->property("Count").toInt();  // 列数

    // step5: 读和写
//    // 读方式一（坐标）：
//    QAxObject* cell = worksheet->querySubObject("Cells(int, int)", i, j);  //获单元格值
//    qDebug() << i << j << cell->dynamicCall("Value2()").toString();
    // 读方式二（行列名称）：
//    QString X = "A" + QString::number(i + 1); //设置要操作的单元格，A1
//    QAxObject* cellX = worksheet->querySubObject("Range(QVariant, QVariant)", X); //获取单元格j
//    qDebug() << cellX->dynamicCall("Value2()").toString();

    // 写方式：
    QString t;
    for(i=1;i<6;i++){
        for(j=1;j<5;j++){
            QAxObject* cell = worksheet->querySubObject("Cells(int, int)", j, i);  //获单元格值   //读方式一
            qDebug() << i << j << cell->dynamicCall("Value2()").toString();
            int m =i+j;
            cell->dynamicCall("SetValue(conts QVariant&)", m); // 设置单元格的值
        }
    }


    // step6: 保存文件
    // 方式一：保存当前文件
//    workbook->dynamicCall("Save()");  //保存文件
//    workbook->dynamicCall("Close(Boolean)", false);  //关闭文件
    // 方式二：另存为
    QString fileName = QFileDialog::getSaveFileName(this, tr("保存文件"), tr("EXCEL(*.xlsx)"));
    workbook->dynamicCall("SaveAs(const QString&)", QDir::toNativeSeparators(fileName)); //保存到filepath
    // 注意一定要用QDir::toNativeSeparators, 将路径中的"/"转换为"\", 不然一定保存不了
    workbook->dynamicCall("Close (Boolean)", false);  //关闭文件

//    QString merge_cell("A1:Z10"); // 设置A1至Z10范围内的单元格的属性
//    QAxObject *merge_range = worksheet->querySubObject("Range(const QString&)", merge_cell);
//    merge_range->setProperty("HorizontalAlignment", -4108); // 水平居中
//    merge_range->setProperty("VerticalAlignment", -4108);  // 垂直居中
//    merge_range->setProperty("NumberFormatLocal", "@");  // 设置为文本

}
