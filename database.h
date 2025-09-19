#ifndef DATABASE_H
#define DATABASE_H

#include<QtSql/QSqlDatabase>
#include<QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

class database
{
public:
    static QSqlDatabase db;

    //connect to sqllite database
    static bool connect();

    //initializees non existing tables
    static void initialize();
};

#endif // DATABASE_H
