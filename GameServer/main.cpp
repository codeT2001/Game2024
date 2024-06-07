#include <QtCore/QCoreApplication>
#include "server.h"
#include "email.h"
#include "mysqlmgr.h"
#include <iostream>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Server::getInstance().start(50050);
    //std::cout << "servce start..." << std::endl;
    //Email::getInstance().sendCode("3308849335@qq.com","12rt","tandingfu","×¢²áÑéÖ¤Âë");
    return a.exec();
}
