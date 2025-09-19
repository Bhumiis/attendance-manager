#include "database.h"

QSqlDatabase database::db;

bool database::connect()
{
    if (QSqlDatabase::contains("qt_sql_default_connection")) {
        db = QSqlDatabase::database("qt_sql_default_connection");
    } else {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("attendance.db"); // file in project folder
    }

    if (!db.open()) {
        qDebug() << "Error: Could not connect to database -" << db.lastError().text();
        return false;
    }

    qDebug() << "Database connected successfully!";
    return true;
}

void database::initialize()
{
    QSqlQuery query;


    // Drop old tables
   //query.exec("DROP TABLE IF EXISTS employee");
    //query.exec("DROP TABLE IF EXISTS attendance");

    //  Create Employees table (singular name to match employee.cpp)
    query.exec("CREATE TABLE IF NOT EXISTS employee ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "name TEXT NOT NULL,"
               "department TEXT)");

    //  Create Attendance table
    query.exec("CREATE TABLE IF NOT EXISTS attendance ("
               "id INTEGER PRIMARY KEY AUTOINCREMENT,"
               "employeeId INTEGER,"
               "date TEXT,"
               "status TEXT,"
               "FOREIGN KEY(employeeId) REFERENCES employee(id))");

    qDebug() << "Tables initialized!";
}
