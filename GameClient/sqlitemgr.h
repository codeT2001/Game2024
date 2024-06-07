#ifndef SQLITEMGR_H
#define SQLITEMGR_H
#include <QtSql/QSqlDriver>
#include <QtSql/QSqlError>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include "player.h"
class SqliteMgr
{
public:
    static SqliteMgr& getInstance()
    {
        static SqliteMgr instance;
        return instance;
    }
    ~SqliteMgr();
    bool loadApply(QList<Apply*>& list);
    void getChessGame(QList<QString>& list);
    void saveChessGame(QList<Item>& list);
    void loadChessGame(QList<Item>& list,QString timestr);
    void saveApplys(QList<Apply*>&list);
private:
    SqliteMgr();
private:
    QSqlDatabase m_db;
    QSqlQuery* m_query;
};

#endif // SQLITEMGR_H
