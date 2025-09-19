#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "employee.h"
#include "attendance.h"
#include <QStandardItem>
#include <QStandardItemModel>

#include <QDate>
#include <QMessageBox>

// Define Light & Dark Styles
const char *lightStyle = R"(
    QMainWindow { background-color: #f5f6fa; }
    QPushButton {
        background-color: #3498db;
        color: white;
        border-radius: 5px;
        padding: 5px 10px;
    }
    QPushButton:hover { background-color: #2980b9; }
    QTableView {
        background: white;
        alternate-background-color: #ecf0f1;
        gridline-color: #bdc3c7;
    }
)";

const char *darkStyle = R"(
    QMainWindow { background-color: #2c3e50; }
    QPushButton {
        background-color: #1abc9c;
        color: white;
        border-radius: 5px;
        padding: 5px 10px;
    }
    QPushButton:hover { background-color: #16a085; }
    QTableView {
        background: #34495e;
        alternate-background-color: #2c3e50;
        color: white;
        gridline-color: #7f8c8d;
    }
    QLabel, QHeaderView::section { color: white; }
)";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Add theme menu
    QMenu *viewMenu = menuBar()->addMenu("View");
    QAction *toggleThemeAction = new QAction("Toggle Dark Mode", this);
    viewMenu->addAction(toggleThemeAction);
    connect(toggleThemeAction, &QAction::triggered, this, &MainWindow::toggleTheme);

    // Start with Light Theme
    qApp->setStyleSheet(lightStyle);

    // Setup employees table model
    modelEmployees = new QStandardItemModel(this);
    modelEmployees->setHorizontalHeaderLabels({"ID", "Name", "Department"});
    ui->tableEmployees->setModel(modelEmployees);
    ui->tableEmployees->horizontalHeader()->setStretchLastSection(true);
    ui->tableEmployees->setAlternatingRowColors(true);
    ui->tableEmployees->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableEmployees->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableEmployees->setSelectionMode(QAbstractItemView::SingleSelection);


    // Load employees from database
    loadEmployees();

    // Setup combo box for attendance status
    ui->comboStatus->addItems({"present", "absent", "late"});

    // Connect calendar to attendance fetch
      connect(ui->calendarWidget, &QCalendarWidget::clicked, this, [=](const QDate &date)
      {
          QList<attendance> records = attendance::getByDate(date);
          QString summary;
          for (const attendance &att : records)
          {
              summary += QString("Employee %1: %2\n")
                             .arg(att.employeeId)
                             .arg(att.status);
          }
          QMessageBox::information(this, "Attendance", summary.isEmpty() ? "No records" : summary);
      });

}


MainWindow::~MainWindow()
{
    delete ui;
}

// Load employees into QTableView
void MainWindow::loadEmployees()
{
    modelEmployees->removeRows(0, modelEmployees->rowCount());
    QList<employee> employees = employee::getAll();

    for (const employee &emp : employees)
    {
        QList<QStandardItem*> row;
        row << new QStandardItem(QString::number(emp.id));
        row << new QStandardItem(emp.name);
        row << new QStandardItem(emp.department);
        modelEmployees->appendRow(row);
    }

}

// Add new employee
void MainWindow::on_btnAddEmployee_clicked()
{
    QString name = ui->lineName->text().trimmed();
    QString dept = ui->lineDepartment->text().trimmed();
    if (name.isEmpty() || dept.isEmpty())
    {
        QMessageBox::warning(this, "Error", "Name and Department cannot be empty.");
        return;
    }

    employee e(0, name, dept);
    e.save();
    loadEmployees();

    ui->lineName->clear();
    ui->lineDepartment->clear();
}

void MainWindow::showSummaryChart()
{
    QList<attendance> today = attendance::getByDate(QDate::currentDate());

    int present = 0, absent = 0, late = 0;
    for (const attendance &att : today)
    {
        if (att.status == "present") present++;
        else if (att.status == "absent") absent++;
        else if (att.status == "late") late++;
    }

    QPieSeries *series = new QPieSeries();
    series->append("Present", present);
    series->append("Absent", absent);
    series->append("Late", late);

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Today's Attendance Summary");

    QChartView *chartView = new QChartView(chart);
    chartView->setRenderHint(QPainter::Antialiasing);

    // Show in a standalone window
    chartView->resize(400, 300);
    chartView->show();
}

// Mark attendance for selected employee
void MainWindow::on_btnMarkAttendance_clicked()
{
    QModelIndex index = ui->tableEmployees->currentIndex();
    if (!index.isValid())
    {
        QMessageBox::warning(this, "Error", "Select an employee from the table first.");
        return;
    }

    int row = index.row();
    int empId = modelEmployees->item(row, 0)->text().toInt();
    QString status = ui->comboStatus->currentText();

    attendance att(empId, QDate::currentDate(), status);
    if (att.mark())
    {
        QMessageBox::information(this, "Success", "Attendance marked for employee.");
         showSummaryChart();
    }
    else
    {
        QMessageBox::warning(this, "Info", "Attendance already marked for today.");
    }
}

//theme choosing option
void MainWindow::toggleTheme()
{
    static bool dark = false;
    dark = !dark;
    if (dark)
        qApp->setStyleSheet(darkStyle);
    else
        qApp->setStyleSheet(lightStyle);
}


