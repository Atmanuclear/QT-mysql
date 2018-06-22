#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtSql>
#include <QSqlQuery>
#include <QMessageBox>

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("10.236.51.145"); //10.236.51.14
        db.setPort(3306);
        db.setDatabaseName("qtest");
        db.setUserName("admin");
        db.setPassword("admin");
       /* QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
            db.setHostName("10.236.51.145"); //10.236.51.14
            db.setPort(3306);
            db.setDatabaseName("qtest");
            db.setUserName("admin");
            db.setPassword("admin");
            */
    if(!db.open()) {
        QMessageBox::critical(0, "Cannot open database",
                              "Unable to establish a database connection.", QMessageBox::Cancel);
        return false;
    }
    QSqlQuery query;
    query.exec("create table student (id int primary key, name vchar)");
    query.exec("insert into student values (0,'刘明')");
    query.exec("insert into student values (1,'陈刚')");
    query.exec("insert into student values (2,'王红')");
    return true;
}

#endif // CONNECTION_H


//git 测试 123
