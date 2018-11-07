#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <cmath>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

// середня кількість рядків для функцій (методів)
double MainWindow::AvgNumLinFun() {

    // записуємо регулярне вираження та ділимо текст
    auto rx = QRegExp("(void|int|string|float|double|byte|bool)\\s+(\\w+::)?\\w+\\(.*\\)");
    rx.setMinimal(true);
    QStringList funs = code.split(rx,  QString::SkipEmptyParts);
    double lines = 0;
    QString fun;

    // розраховуємо загальну кількість рядків
    for(int i = 1; i < funs.count(); i++) {
        lines += funs[i].count("\n");
    }

    // розраховуємо середнє значення
    return funs.count() == 1 ? 0 : lines / (funs.count() - 1);
}

// середня кількість рядків, що містять вихідний код для функцій (методів)
double MainWindow::AvgNumLinCode() {

    // записуємо регулярне вираження та ділимо текст
    auto rx = QRegExp("(void|int|string|float|double|byte|bool)\\s+(\\w+::)?\\w+\\(.*\\)");
    rx.setMinimal(true);
    QStringList funs = code.split(rx,  QString::SkipEmptyParts);
    double lines = 0;
    QString fun;
    int empty_line, comment = 0;

    // видаляємо пусті рядки
    for(int i = 1; i < funs.count(); i++) {
        empty_line = funs[i].indexOf(QRegExp("\\n\\s*\\n"));
        while(empty_line != -1) {
            funs[i] = funs[i].remove(empty_line, 1);
            empty_line = funs[i].indexOf(QRegExp("\\n\\s*\\n"));
        }
    }

    // розраховуємо кількість рядків тільки з коментарями
    for(int i = 1; i < funs.count(); i++) {
        comment += funs[i].count(QRegExp("\n\\s*//"));
    }

    // розраховуємо загальну кількість рядків
    for(int i = 1; i < funs.count(); i++) {
        lines += funs[i].count("\n");
    }

    // розраховуємо середнє значення
    return funs.count() == 1 ? 0 : (lines - comment) / (funs.count() - 1);
}

// середня кількість рядків для модулів
double MainWindow::AvgNumLinMod() {
    QStringList lst = code.split(QRegExp("//module"),  QString::SkipEmptyParts);
    double s = 0;
    if(lst.count() != 1) {
        for(int i = lst.count()-1; i >= 0; --i) {
            const QString& item = lst[i];
            s=s+item.count("\n")-1;
        }
    }
    return s / (lst.count());
}

// кількість рядків "#include"
int MainWindow::NumLinInc() {

    // видаляємо рядки до першого "#include"
    QString lokal_code = code;
    int num_lines = lokal_code.indexOf(QRegExp("#include"));
    lokal_code = lokal_code.remove(0, num_lines);

    // розраховуємо кількість модулів
    QStringList lst = lokal_code.split(QRegExp("#include"),  QString::SkipEmptyParts);
    return lst.count() == 1 ? 0 : lst.count();
}

// середня кількість рядків для класів
double MainWindow::AvgNumLinClass() {
    QStringList lst = code.split(QRegExp("class "),  QString::SkipEmptyParts);
    double s = 0;
    if(lst.count() != 1) {
        for(int i = lst.count()-1; i >= 0; --i) {
            const QString& item = lst[i];
            s += item.count("\n");
        }
    }
    return s/(lst.count());
}

// кількість викликів віддалених методів (Number Of Remote Methods, NORM)
int MainWindow::NORM() {
    QStringList lst = code.split(QRegExp("class "),  QString::SkipEmptyParts);
    int s = 0;
    for(int i = lst.count()-1; i >= 0; --i) {
        const QString& item = lst[i];
        QStringList list = item.split("){",  QString::SkipEmptyParts);
        s=s+item.count(") ")-item.count("this.")-item.count(" ) ");
    }
    return s;
}

// відгук на клас (Response For Class, RFC)
int MainWindow::RFC() {
    QStringList lst = code.split(QRegExp("class "),  QString::SkipEmptyParts);
    int s = 0;
    for(int i = lst.count()-1; i >= 0; --i) {
        const QString& item = lst[i];
        QStringList list = item.split("){",  QString::SkipEmptyParts);
        s=s+item.count(") ")-item.count(" ) ");
    }
    return s;
}

// зважена насиченість класу (Weighted Methods Per Class, WMC)
int MainWindow::WMC() {
    QStringList lst = code.split(QRegExp("class "),  QString::SkipEmptyParts);
    int s = 0;
    for(int i = lst.count()-1; i >= 0; --i) {
        const QString& item = lst[i];
        QStringList list = item.split("){",  QString::SkipEmptyParts);
        s=0;
        for(int j = list.count()-1; j >= 0; --j) {
            const QString& ite = list[j];
            if ((ite.count("{")>=ite.count("}"))&&(ite.count("}")!=0))
                s += 1;
        }
        return s;
    }
    return 0;
}

// недолік зв’язності методів (Lack of cohesion in Methods, LCOM)
double MainWindow::LCOM() {
    int P = 0, Q = 0;
    int m;
    QString coual = "";
    QStringList lst = code.split(QRegExp("class "),  QString::SkipEmptyParts);
    int s = 0;
    for(int i = lst.count()-1; i >= 0; --i) {
        P = 0;
        Q = 0;
        const QString& item = lst[i];
        QStringList list = item.split("){",  QString::SkipEmptyParts);
        s=0;
        for(int j = list.count()-1; j >= 0; --j){
            const QString& ite = list[j];
            QStringList lt = ite.split(QRegExp("(void|int|string|float|double|byte)[A-Za-z0-9]*"),  QString::SkipEmptyParts);
            lt.removeDuplicates();
            m = lt.count();
            for(int g = lt.count()-1; g >= 1; --g){
                const QString& ir = lt[g];
                QStringList l = ir.split(" ");
                QString dd="+-*=:/%()";
                for(int c = l.count()-1; c >= 1; --c){
                    const QString& r = l[c];
                    if(dd.count(r)==0)
                        coual=coual+r;
                    coual=coual+"++";
                }
            }
        }
        QStringList vars = coual.split("++");
        for(int h = vars.count()-1; h >= 1; --h){
            const QString& qw = vars[h];
            int y=0;
            for(int k = h; k >= 0; --k){
                const QString& qw1 = vars[k];
                if (qw==qw1)
                    {P=P+1; break;}
                else
                    y=y+1;
                if ((k==0)&&(y==vars.count()-1-h))
                    Q=Q+1;
            }
        }
        float C = 0;
        float f1=1;
        float f2=1;
        for (int z=1; z<=m; z++)
            f1=f1*z;
        for (int z=1; z<=m-2; z++)
            f2=f2*z;
        C = sqrt(f1/2/f2);
        C = (P-Q)/C;
        return C;
    }
    return 0;
}

// кількість нащадків (Number of children, NOC)
int MainWindow::NOC(){

    // делим по классам
    QStringList class_list = code.split(QRegExp("class\\s"),  QString::SkipEmptyParts),
                class_decl;
    int count = 0;

    // заносим в список объявления классов (1-ую строку)
    for (int el = 0; el < class_list.count(); el++) {
        QString item = class_list[el].trimmed();
        int index = item.indexOf("\n") != -1 ? item.indexOf("\n") : item.count();
        QString res = item.remove(index, item.count() - index + 1);
        class_decl << res;
    }

    // смотрим, является ли класс наследником
    for (int el = 0; el < class_decl.count(); el++) {
        QStringList children = class_decl[el].split(QRegExp("public|protected|private "),  QString::SkipEmptyParts);
        if(children.count() > 1) {
            count ++;
        }
    }
    return count;
}

// глибина дерева спадкування (Depth of Inheritance tree, DIT)
int MainWindow::DIT() {
    QStringList lst = code.split(QRegExp("class\\s"),  QString::SkipEmptyParts);
    QStringList lis, Lclss, Rclss;
    for (int g = 0; g<lst.count(); g++) {
        const QString& item = lst[g].trimmed();
        QString St ="";
        for(int i=0; i<item.count(); i++)
            if (item[i]!='\n')
                St=St+QString(item[i]);
            else break;
        lis<<St;
    }
    for (int g = 0; g<lis.count(); g++) {
        const QString& item = lis[g].trimmed();
        QString St ="";
        for(int i=0; i<item.count(); i++)
            if (item[i]!=' ')
                St=St+QString(item[i]);
            else break;
               Rclss<<St;
    }
    for (int g = 0; g<lis.count(); g++) {
        const QString& item = lis[g].trimmed();
        QString St ="";
        lst = item.split(QRegExp("public|protected|private "),  QString::SkipEmptyParts);
        for(int i=0; i<lst.count(); i++)
            St = St+lst[i];
        Lclss<<St;
    }
    for (int g = 0; g<Rclss.count(); g++) {
        Lclss[g].replace(0,Rclss[g].count(),"");
        Lclss[g] = Lclss[g].trimmed();
    }
    for (int g = 0; g<Rclss.count(); g++) {
        Rclss[g].remove(":");
        Lclss[g].remove(",");
    }
    int** mass=new int*[Rclss.count()+1];
    for(int i=0; i<=Rclss.count();i++)
        mass[i]= new int [Rclss.count()+1];
    for (int g = 0; g<Rclss.count(); g++)
        for (int j = 0; j<Rclss.count(); j++) {
            mass[g][j]=0;
            if (g!=j)
                {mass[g][j] = Lclss[g].count(Rclss[j]);}
        }
    int pst = 0;
    int prw = -1;
    int sum =0;
    int max = 0;
    for (int i = 0; i<Rclss.count(); i++) {
        prw=i;
        pst=0;
        max = 0;
        sum = 0;
        while (pst<Rclss.count()) {
            int k;
            if (mass[prw][pst]==1) {
                if (prw == i)
                    k=pst;
                mass[prw][pst]=0;
                prw=pst;
                pst=0;
                sum=sum+1;
            }
            else pst=pst+1;
            if (max<sum)
                max=sum;
            if ((pst==Rclss.count())&&(prw!=i)) {
                prw = i;
                pst=0;
                sum=0;
            }
            if ((pst==Rclss.count())&&(prw==i))
                break;
        }
        for (int g = 0; g<Rclss.count(); g++)
            for (int j = 0; j<Rclss.count(); j++) {
                mass[g][j]=0;
                if (g!=j)
                    mass[g][j] = Lclss[g].count(Rclss[j]);
            }
        return max;
    }
    return 0;
}

void MainWindow::on_load_clicked() {
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open File"), "",tr("C++ Files (*.cpp *.h)"));
        if (fileName != "") {
                QFile file(fileName);
                if (!file.open(QIODevice::ReadOnly)) {
                    QMessageBox::critical(this, tr("Error"), tr("Could not open file"));
                    return;
                }
                QTextStream in(&file);
                ui->plainTextEdit->setPlainText(in.readAll());
                file.close();
        }
        code = ui->plainTextEdit->toPlainText();
}

void MainWindow::on_result_clicked() {
    code = "\n" + ui->plainTextEdit->toPlainText() + "\n";
    ui->anlf->setText(QString::number(AvgNumLinFun()));
    ui->anlc->setText(QString::number(AvgNumLinCode()));
    ui->module->setText(QString::number(AvgNumLinMod()));
    ui->inc->setText(QString::number(NumLinInc()));
    ui->class_2->setText(QString::number(AvgNumLinClass()));
    ui->norm->setText(QString::number(NORM()));
    ui->rfc->setText(QString::number(RFC()));
    ui->wmc->setText(QString::number(WMC()));
    ui->lcom->setText(QString::number(LCOM()));
    ui->noc->setText(QString::number(NOC()));
    ui->dit->setText(QString::number(DIT()));
}
