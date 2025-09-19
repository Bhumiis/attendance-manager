#include "attendance.h"
#include "database.h"
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlError>
#include <QDebug>

attendance::attendance() {
    id = 0;
    employeeId = 0;
    date = QDate::currentDate();
    status = "absent";   // default
}

attendance::attendance(int eid, QDate d, QString s) {
    id = 0;
    employeeId = eid;
    date = d;
    status = s.toLower();   // normalize to lowercase
}

bool attendance::mark() {
    QSqlQuery check;
    check.prepare("SELECT COUNT(*) FROM attendance WHERE employeeId=? AND date=?");
    check.addBindValue(employeeId);
    check.addBindValue(date.toString("yyyy-MM-dd"));
    check.exec();
    check.next();

    if (check.value(0).toInt() > 0) {
        qDebug() << "Attendance already marked for employee" << employeeId << "on" << date.toString();
        return false;
    }

    QSqlQuery query;
    query.prepare("INSERT INTO attendance (employeeId, date, status) VALUES (?, ?, ?)");
    query.addBindValue(employeeId);
    query.addBindValue(date.toString("yyyy-MM-dd"));
    query.addBindValue(status);

    if (!query.exec()) {
        qDebug() << "Error marking attendance:" << query.lastError().text();
        return false;
    }
    return true;
}

QList<attendance> attendance::getByDate(QDate d) {
    QList<attendance> list;
    QSqlQuery query;
    query.prepare("SELECT id, employeeId, date, status FROM attendance WHERE date=?");
    query.addBindValue(d.toString("yyyy-MM-dd"));
    query.exec();

    while (query.next()) {
        attendance att;
        att.id = query.value(0).toInt();
        att.employeeId = query.value(1).toInt();
        att.date = QDate::fromString(query.value(2).toString(), "yyyy-MM-dd");
        att.status = query.value(3).toString().toLower();
        list.append(att);
    }
    return list;
}

QList<attendance> attendance::getByEmployee(int eid) {
    QList<attendance> list;
    QSqlQuery query;
    query.prepare("SELECT id, employeeId, date, status FROM attendance WHERE employeeId=?");
    query.addBindValue(eid);
    query.exec();

    while (query.next()) {
        attendance att;
        att.id = query.value(0).toInt();
        att.employeeId = query.value(1).toInt();
        att.date = QDate::fromString(query.value(2).toString(), "yyyy-MM-dd");
        att.status = query.value(3).toString().toLower();
        list.append(att);
    }
    return list;
}
