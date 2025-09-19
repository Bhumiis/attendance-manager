#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStandardItemModel>
#include <QtCharts/QChartView>
#include <QtCharts/QPieSeries>
#include <QtCharts/QChart>


QT_CHARTS_USE_NAMESPACE   // allows direct use of QChart, QPieSeries etc.


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_btnAddEmployee_clicked();

    void on_btnMarkAttendance_clicked();

    void toggleTheme();  // slot to switch themes

private:
    Ui::MainWindow *ui;

    QStandardItemModel *modelEmployees;

    void loadEmployees();

    void showSummaryChart();
};
#endif // MAINWINDOW_H
