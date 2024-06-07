#include "sqlitemgr.h"
#include <qDebug>
#include <QDateTime>
SqliteMgr::~SqliteMgr()
{
    delete m_query;
}

bool SqliteMgr::loadApply(QList<Apply *> &list)
{
    QString sql = QString("select * from applys where id='%1' order by applytime DESC limit 20");
    qDebug() << sql;
    m_query->exec(sql);
    while (m_query->next())
    {
        int applyId = m_query->value(0).toInt();
        QString applyName = m_query->value(1).toString();
        QString applyTime = m_query->value(2).toString();
        list.append(new Apply{applyId,applyName,applyTime});
    }
    m_query->exec(QString("truncate table applys"));
    return true;
}

void SqliteMgr::getChessGame(QList<QString> &list)
{
    QString sql = QString("select DISTINCT time from chessgame order by time DESC");

    m_query->exec(sql);
    while(m_query->next())
    {
        list<<m_query->value(0).toString();
        qDebug()<<m_query->value(0).toString();
    }
}

void SqliteMgr::saveChessGame(QList<Item> &list)
{

    QString sql = QString("insert into chessgame values(?,?,?,?,?)");
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
    for(int i=0;i<list.size();++i)
    {
        auto& t = list.at(i);
        m_query->prepare(sql);
        m_query->addBindValue(time);
        m_query->addBindValue(i);
        m_query->addBindValue(t.pt.x());
        m_query->addBindValue(t.pt.y());
        m_query->addBindValue(t.color);
        m_query->exec();
    }


}

void SqliteMgr::loadChessGame(QList<Item> &list, QString timestr)
{

    QString sql = QString("select * from chessgame where time = '%1' order by step ASC").arg(timestr);
    list.clear();
    m_query->exec(sql);
    while(m_query->next())
    {

        int x = m_query->value(2).toInt();
        int y = m_query->value(3).toInt();
        int c = m_query->value(4).toInt();
        list.append(Item{QPoint(x,y),COLOR(c)});
    }

}

void SqliteMgr::saveApplys(QList<Apply *> &list)
{
    QString sql = QString("insert into applys values(?,?,?)");
    for(int i=0;i<list.size();++i)
    {
        auto& a = list.at(i);
        m_query->prepare(sql);
        m_query->addBindValue(a->applyId);
        m_query->addBindValue(a->applyName);
        m_query->addBindValue(a->time);

        m_query->exec();
    }
}
SqliteMgr::SqliteMgr()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    m_db.setDatabaseName("mydatabase.db");
    bool ok = m_db.open();
    if (!ok)
    {
        qDebug() << "数据库连接失败。";
    }
    qDebug() << "数据库连接成功。";
    m_query = new QSqlQuery(m_db);
}
