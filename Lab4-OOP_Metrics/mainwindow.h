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

private:
    Ui::MainWindow *ui;
    QString code;

    double AvgNumLinFun();
    double AvgNumLinCode();
    double AvgNumLinMod();
    int NumLinInc();
    double AvgNumLinClass();
};

#endif // MAINWINDOW_H
