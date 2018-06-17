#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtSql>
#include <QSqlQuery>

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("localhost");
        db.setPort(3306);
        db.setDatabaseName("qtest");
        db.setUserName("root");
        db.setPassword("123456");
    if(!db.open()) return false;
    QSqlQuery query;
    query.exec("create table student (id int primary key, name vchar)");
    query.exec("insert into student values (0,'刘明')");
    query.exec("insert into student values (1,'陈刚')");
    query.exec("insert into student values (2,'王红')");
    return true;
}

#endif // CONNECTION_H


//git 测试 123
