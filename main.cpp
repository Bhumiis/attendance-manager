#include "mainwindow.h"
#include "database.h"
#include "employee.h"
#include "attendance.h"
#include <QApplication>
#include <QDebug>
#include <QDate>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>





int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1. Connect to database
    if (!database::connect()) {
        return -1;
    }

    // 2. Initialize tables (employee + attendance)
    database::initialize();



    // 3 Fetch all remaining employees
    QList<employee> employees = employee::getAll();
    qDebug() << "=== Employees ===";
    for (const employee &emp : employees) {
        qDebug() << emp.id << emp.name << emp.department;
    }

    // 4 Fetch today's attendance
    QList<attendance> today = attendance::getByDate(QDate::currentDate());
    qDebug() << "=== Attendance for today ===";
    for (const attendance &att : today) {
        qDebug() << "EmployeeID:" << att.employeeId
                 << "Date:" << att.date.toString("yyyy-MM-dd")
                 << "Status:" << att.status;
    }

    // 5 Launch GUI
    MainWindow w;
    w.show();

    return a.exec();
}
