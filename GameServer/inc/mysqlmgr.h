#ifndef MYSQLMGR_H
#define MYSQLMGR_H
#include <qsqldatabase.h>
#include <qsqldriver.h>
#include <qsqlerror.h>
#include <qsqlquery.h>
#include "global.h"
#include "player.h"
class MysqlMgr
{
public:
    static MysqlMgr& getInstance()
    {
        static MysqlMgr instance;
        return instance;
    }
    bool findPlayer(QString email, QString pwd, PLAYER* p);
    bool findPlayerById(int id, PLAYER* p);
    bool addPlayer(QString name, QString email, QString pwd);
    bool resetPassword(QString email,QString pwd);
    bool findPlayerByName(QString name, int& pid);
    bool addApply(int id, int applyid,QString applyName,QString time);
    bool addFriends(int id, int friendsId);
    bool loadApplys(int id,QList<Apply>& list);
    bool loadFriends(int id, QList<PLAYER>& list);
    bool deleteFriend(int pid,int delId);
    //bool updateState(int pid, STATES s);
private:
    MysqlMgr();
   
private:
    QSqlDatabase m_db;
    QSqlQuery* m_query;
};

#endif // MYSQLMGR_H
