#include "employee.h"
#include "database.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

employee::employee() {}

employee::employee(int i, QString n, QString d) {
    id = i;
    name = n;
    department = d;
}

// Insert or update employee
// Insert or update employee
bool employee::save() {
    QSqlQuery query;

    if (id == 0) { // new employee
        // 1. Check for duplicates
        query.prepare("SELECT id FROM employee WHERE name=? AND department=?");
        query.addBindValue(name);
        query.addBindValue(department);
        if (!query.exec()) {
            qDebug() << "Error checking duplicate employee:" << query.lastError().text();
            return false;
        }
        if (query.next()) {
            qDebug() << "Employee already exists with ID:" << query.value(0).toInt();
            return false; // duplicate found, do not insert
        }

        // 2. Insert new employee
        query.prepare("INSERT INTO employee(name, department) VALUES (?, ?)");
        query.addBindValue(name);
        query.addBindValue(department);

    } else { // update existing
        query.prepare("UPDATE employee SET name=?, department=? WHERE id=?");
        query.addBindValue(name);
        query.addBindValue(department);
        query.addBindValue(id);
    }

    if (!query.exec()) {
        qDebug() << "Error saving employee:" << query.lastError().text();
        return false;
    }

    return true;
}

// Delete employee
bool employee::remove() {
    if (id == 0) return false;

    QSqlQuery query;
    query.prepare("DELETE FROM employee WHERE id=?");
    query.addBindValue(id);

    if (!query.exec()) {
        qDebug() << "Error deleting employee:" << query.lastError().text();
        return false;
    }
    return true;
}

// Fetch all employee
QList<employee> employee::getAll() {
    QList<employee> list;
    QSqlQuery query("SELECT id, name, department FROM employee");

    while (query.next()) {
        list.append(employee(query.value(0).toInt(),
                             query.value(1).toString(),
                             query.value(2).toString()));
    }
    return list;
}
