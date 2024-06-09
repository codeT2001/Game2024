#include"inc/mainwindow.h"
#include <QApplication>
#include <QFile>
#include <QStyle>
#include "inc/tcpmgr.h"
#include "inc/sqlitemgr.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SqliteMgr::getInstance();
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
    w.show();
    return a.exec();
}
