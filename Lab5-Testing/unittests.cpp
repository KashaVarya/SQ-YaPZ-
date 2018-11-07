#include <QtTest/QtTest>
#include <QtWidgets>
#include "mainwindow.cpp"

class TestMainWindow: public QObject
{
    Q_OBJECT
private slots:
    void AvgNumLinFun_data();
    void AvgNumLinFun();
    void TestRadioButton();
};

void TestMainWindow::AvgNumLinFun_data()
{
    MainWindow mainwindow;
    QTest::addColumn<QString>("code");
    QTest::addColumn<double>("result");

    QString data1 = "#include <cmath>\n"
                    "#include <iostream>\n"
                    "#include <vector>";
    QString data2 = "int fun1(int a) {\n"
                    "// example of function 1\n"
                    "int b = a;\n"
                    "a++;\n"
                    "\n"
                    "return a;\n"
                    "}\n"
                    "\n"
                    "double fun2(int a) {\n"
                    "// example of function 2\n"
                    "double b = a;\n"
                    "// comment 1\n"
                    "// comment 2\n"
                    "a++;\n"
                    "// comment 3\n"
                    "\n"
                    "return a;\n"
                    "}";

    QTest::newRow("no func")     << data1 << 0.0;
    QTest::newRow("two funcs")    << data2 << 9.0;
}

void TestMainWindow::AvgNumLinFun()
{
    QFETCH(QString, code);
    QFETCH(double, result);

    MainWindow mainwindow;
    mainwindow.code = code;

    QCOMPARE(mainwindow.AvgNumLinFun(), result);
}

void TestMainWindow::TestRadioButton()
{
    QRadioButton radioload;
    radioload.show();
    QCOMPARE(radioload.isChecked(), false);
    QTest::mouseClick(&radioload, Qt::LeftButton);
    QCOMPARE(radioload.isChecked(), true);
}

QTEST_MAIN(TestMainWindow)
#include "unittests.moc"
