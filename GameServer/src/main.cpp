#include <QtCore/QCoreApplication>
#include "inc/server.h"
#include "inc/email.h"
#include "inc/mysqlmgr.h"
#include <iostream>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server::getInstance().start(50050);
    return a.exec();
}
