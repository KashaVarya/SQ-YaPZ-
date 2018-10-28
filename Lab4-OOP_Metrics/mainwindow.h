#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_result_clicked();
    void on_load_clicked();

private:
    Ui::MainWindow *ui;
    QString code;

    double AvgNumLinFun();
    double AvgNumLinCode();
    double AvgNumLinMod();
    int NumLinInc();
    double AvgNumLinClass();
    int NORM();
    int RFC();
    int WMC();
    double LCOM();
    int NOC();
    int DIT();
};

#endif // MAINWINDOW_H
