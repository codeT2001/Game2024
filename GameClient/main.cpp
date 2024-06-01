#include "mainwindow.h"
#include "gamedialog.h"
#include <QApplication>
#include <QFile>
#include <QStyle>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QFile qss(":/styles/styleSheet.qss");
    if(qss.open(QFile::ReadOnly))
    {
        qDebug()<<"open success.";
        QString style = qss.readAll();
        a.setStyleSheet(style);
        qss.close();
    }
    else
    {
        qDebug()<<"open failed.";
    }
    MainWindow w;
    //GameDialog w;
    w.show();
    return a.exec();
}
