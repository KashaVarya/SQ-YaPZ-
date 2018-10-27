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
    QString text;
    int all_lines,
        empty_lines,
        com_lines;

    void count_lines();
    void count_empty_lines();
    void count_comment_lines();
    void count_code_lines();
    void count_code_com_lines();
    void count_voc_operators();
    void count_voc_operands();
    void count_all_operators();
    void count_all_operands();
    void count_length();
    void count_V();
    void coutn_cyclomatic();
    void count_voc();
    void count_CI();
};

#endif // MAINWINDOW_H
