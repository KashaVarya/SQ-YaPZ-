#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QString>
#include <QLocale>
#include <QStringList>
#include <QList>
#include <QRegExp>
#include <qmath.h>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

// Рассчитываем общее количество строк
void MainWindow::count_lines() {
    all_lines = text.count("\n") - 1;
    ui->all->setText(QString::number(all_lines));
}

// Рассчитываем количество пустых строк
void MainWindow::count_empty_lines() {
    empty_lines = text.count("\n\n");
    ui->empty->setText(QString::number(empty_lines));
}

// Рассчитываем количество строк, содержащих комментарии
void MainWindow::count_comment_lines() {
    com_lines = text.count("\n//");
    ui->comment->setText(QString::number(com_lines));
    ui->comment_per->setText(QString::number((double(com_lines)) / all_lines * 100) + "%");
}

// Рассчитываем количество строк чистого кода (общее количество строк - комментарии - пустые строки)
void MainWindow::count_code_lines() {
    ui->code->setText(QString::number(all_lines - empty_lines - com_lines));
}

// Рассчитываем количество строк кода с комментариями
void MainWindow::count_code_com_lines() {
    int code_com_lines = 0;
    code_com_lines = text.count("//");
    ui->code_com->setText(QString::number(code_com_lines - com_lines));
    ui->com_code_per->setText(QString::number((double(code_com_lines)) / all_lines * 100) + "%");
}

int n1, n2, m, N1, N2, N, R;

// Рассчитываем словарь операторов (количество уникальных операторов программы, также символы-разделители, имена процедур и знаки операций)
void MainWindow::count_voc_operators() {
    QStringList list = text.split(" ");
    list.removeDuplicates();
    QString types = "+ - = += ++ -- * << >> < > != == || && &";
    QStringList ltypes, res;
    ltypes = types.split(" ");
    QStringList::Iterator itt;

    for(int i = list.count()-1; i >= 0; --i) {
        const QString& item = list[i];
        itt = ltypes.begin();
        while (itt != ltypes.end()) {
            if(item == *itt){
                res << item;
            }
            ++itt;
        }
    }

    int r = 0;
    QStringList lst = text.split(QRegExp("(void|int|string|float|double|byte) [A-ZА-z0-9]*"),  QString::SkipEmptyParts);
    for(int i = lst.count()-1; i >= 0; --i) {
        const QString& item = lst[i];
        r += item.count(")\n");
    }
    R = r;
    n1 = res.count();
    ui->voc_operators->setText(QString::number(res.count()+r));
}

// Рассчитываем словарь операндов (количество уникальных операндов программы)
void MainWindow::count_voc_operands() {
    QRegExp rx("([\\w\\']+)[\\s,.;]");
    QStringList list;
    int pos = 0;
    while ((pos = rx.indexIn(text, pos)) != -1) {
        list << rx.cap(1);
        pos += rx.matchedLength();
    }
    list.removeDuplicates();
    QString types = "int void string double float iterator List const array for while do goto";
    QStringList ltypes;
    ltypes = types.split(" ");
    QStringList::Iterator itt = ltypes.begin();
    for(int i = list.count()-1; i >= 0; --i) {
        const QString& item = list[i];
        itt = ltypes.begin();
        while (itt != ltypes.end()){
            if(item == *itt)
                list.removeAt(i);
            ++itt;
        }
        if ((item.toDouble() != 0.0)) {
            list.removeAt(i);
        }
    }

    for(int i = list.count()-1; i >= 0; --i) {
        const QString& item = list[i];
        if (item == "0") {
            list.removeAt(i);
        }
    }
    n2=list.count();
    ui->voc_operands->setText(QString::number(list.count()));
}

// Рассчитываем общее количество операторов в программе
void MainWindow::count_all_operators() {
    QStringList list = text.split(" ");
    QString types = "+ - = += ++ -- * << >> < > != == || && &";
    QStringList ltypes;
    QStringList ress;
    ltypes = types.split(" ");
    QStringList::Iterator itt = ltypes.begin();
    for(int i = list.count()-1; i >= 0; --i) {
        const QString& item = list[i];
        itt = ltypes.begin();
        while (itt != ltypes.end()){
            if(item==*itt){
                ress << item;
            }
            ++itt;
        }
    }
    N1 = ress.count();
    ui->all_opr->setText(QString::number(ress.count()+R));
}

// Рассчитываем общее количество операндов в программе
void MainWindow::count_all_operands() {
    QRegExp rx("([\\w\\']+)[\\s,.;]");
    QStringList list;
    int pos = 0;
    while ((pos = rx.indexIn(text, pos)) != -1) {
        list << rx.cap(1);
        pos += rx.matchedLength();
    }
    QString types = "int void string double float iterator List const array for while do goto";
    QStringList ltypes;
    ltypes = types.split(" ");
    QStringList::Iterator itt = ltypes.begin();
    for(int i = list.count()-1; i >= 0; --i)
    {
        const QString& item = list[i];
        itt = ltypes.begin();
        while (itt != ltypes.end()){
            if(item==*itt)
                list.removeAt(i);
            ++itt;
        }
        if ((item.toDouble() != 0.0)) {
            list.removeAt(i);
        }
    }
    for(int i = list.count()-1; i >= 0; --i) {
        const QString& item = list[i];
        if (item == "0") {
            list.removeAt(i);
        }
    }
    N2 = list.count();
    ui->all_opd->setText(QString::number(list.count()));
}

// Рассчитываем словарь программы m = n1 + n2 + R
void MainWindow::count_voc() {
    m = n1 + n2 + R;
    ui->vocabulary->setText(QString::number(m));
}

// Рассчитываем длину программы N = N1 + N2 + R
void MainWindow::count_length() {
    N = N1 + N2 + R;
    ui->length->setText(QString::number(N));
}

// Рассчитываем объем программы N*log2m
void MainWindow::count_V() {
    double q =  N * qLn(m) / qLn(2);
    ui->v->setText(QString::number(q));
}

// Рассчитываем цикломатическую сложность
void MainWindow::coutn_cyclomatic() {
    QString text= ui->plainTextEdit->toPlainText();
    QStringList list = text.split(QRegExp("(for|while)"),  QString::SkipEmptyParts);
    int s=0;
    for(int i = list.count()-1; i >= 0; --i) {
        const QString& item = list[i];
        if ((item.count("{") >= item.count("}")) && (item.count("}") != 0)) {
            s=s+1;
        }
    }
    ui->cyclomatic->setText(QString::number(s+text.count("if(")));
}

// Абсолютная сложность по Джилбу - количество условных операторов
void MainWindow::count_CI() {
    int n = text.count("if(");
    ui->ci->setText(QString::number(n));
}

void MainWindow::on_result_clicked() {
    text = "\n" + ui->plainTextEdit->toPlainText() + "\n";
    count_lines();
    count_empty_lines();
    count_comment_lines();
    count_code_lines();
    count_code_com_lines();
    count_voc_operators();
    count_voc_operands();
    count_all_operators();
    count_all_operands();
    count_voc();
    count_length();
    count_V();
    coutn_cyclomatic();
    count_CI();
}
