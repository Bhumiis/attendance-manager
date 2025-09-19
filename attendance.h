#ifndef ATTENDANCE_H
#define ATTENDANCE_H

#include<QString>
#include<QDate>
#include<QList>

class attendance
{
public:
    int id;
       int employeeId;
       QDate date;
       QString status;  // Present, Absent, Late

       attendance();
       attendance(int employeeId, QDate date, QString status);

       bool mark();  // Insert attendance

       bool remove();
       static QList<attendance> getByDate(QDate date);  // Fetch by day
       static QList<attendance> getByEmployee(int employeeId); // Fetch for one employee
};

#endif // ATTENDANCE_H
