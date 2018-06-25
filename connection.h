#ifndef CONNECTION_H
#define CONNECTION_H

#include <QtSql>
#include <QSqlQuery>
#include <QMessageBox>

static bool createConnection()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
        db.setHostName("localhost");
        db.setPort(3306);
        db.setDatabaseName("qtest");
        db.setUserName("root");
        db.setPassword("123456");
    if(!db.open()) {
        QMessageBox::critical(0, "Cannot open database",
                              "Unable to establish a database connection.", QMessageBox::Cancel);
        return false;
    }
    return true;
}

#endif // CONNECTION_H


//git 测试 123
