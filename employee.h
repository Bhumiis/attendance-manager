#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include<QString>
#include<QList>
class employee
{
public:
    int id;
    QString name;
    QString department;

    employee();  // default constructor
    employee(int id, QString name, QString department);

    // Database operations
    bool save();                   // Add or update employee
    bool remove();                 // Delete employee
    static QList<employee> getAll(); // Fetch all employee
};

#endif // EMPLOYEE_H
