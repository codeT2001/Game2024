#include "inc/mysqlmgr.h"
#include <iostream>

MysqlMgr::MysqlMgr():m_query(nullptr)
{
	m_db = QSqlDatabase::addDatabase("QMYSQL");
	m_db.setHostName("127.0.0.1");
	m_db.setPort(3306);
	m_db.setDatabaseName("WzQGame");
	m_db.setUserName("root");
	m_db.setPassword("123456");
	bool ok = m_db.open();
	if (!ok)
	{
		std::cout << "数据库连接失败。" << std::endl;
	}
	std::cout << "数据库连接成功。" << std::endl;
	m_query = new QSqlQuery(m_db);
}

bool MysqlMgr::findPlayer(QString email, QString pwd,PLAYER *p)
{
	QString sql = QString("select * from player where email='%1' and pwd = '%2'")
		.arg(email).arg(pwd);
	m_query->exec(sql);
	//找到玩家
	if (m_query->next())
	{
		p->id = m_query->value(0).toInt();
		p->name = m_query->value(1).toString();
		p->email = m_query->value(2).toString();
		p->passwd = m_query->value(3).toString();
		//p->state = STATES(m_query->value(4).toInt());
		return true;
	}
	return false;
}

bool MysqlMgr::findPlayerById(int id, PLAYER* p)
{
	QString sql = QString("select * from player where uid = '%1'")
		.arg(id);
	m_query->exec(sql);
	//找到玩家
	if (m_query->next())
	{
		p->id = m_query->value(0).toInt();
		p->name = m_query->value(1).toString();
		p->email = m_query->value(2).toString();
		p->passwd = m_query->value(3).toString();
		//p->state = STATES(m_query->value(4).toInt());
		return true;
	}
	return false;
}

bool MysqlMgr::addPlayer(QString name, QString email, QString pwd)
{
	m_query->prepare("insert into player (name,email,pwd) values(?,?,?)");
	m_query->addBindValue(name);
	m_query->addBindValue(email);
	m_query->addBindValue(pwd);
	
	if (m_query->exec())
	{
		std::cout << "insert success" << std::endl;
		return true;
		
	}
	else
	{
		std::cout << "Insert failed:" << m_query->lastError().text().toStdString();
		return false;
	}
}

bool MysqlMgr::resetPassword(QString email, QString pwd)
{
	m_query->prepare("update player set pwd = ? where email = ?");
	m_query->addBindValue(pwd);
	m_query->addBindValue(email);

	if(m_query->exec())
	{
		if (!m_query->numRowsAffected())
		{
			std::cout << "update failed" << std::endl;
			return false;
		}
		std::cout << "update success" << std::endl;
		return true;
	}
	else
	{
		std::cout << "update error" << m_query->lastError().text().toStdString() <<std::endl;
		return false;
	}

}

bool MysqlMgr::findPlayerByName(QString name, int& pid)
{
	QString sql = QString("select uid from player where name = '%1'")
		.arg(name);
	std::cout << sql.toStdString() << std::endl;
	m_query->exec(sql);
	//找到玩家
	if (m_query->next())
	{
		pid = m_query->value(0).toInt();
		return true;
	}
	return false;
}

bool MysqlMgr::addApply(int id, int applyid, QString applyName, QString time)
{
	QString sql = QString("select * from applys where id='%1' and applyId = '%2'")
		.arg(id).arg(applyid);
	std::cout << sql.toStdString() << std::endl;
	m_query->exec(sql);

	if (!m_query->next())
	{
		 sql = QString("insert into applys values('%1','%2','%3','%4')")
			.arg(id).arg(applyid).arg(applyName).arg(time);
	}
	else
	{
		sql = QString("update  applys set applyTime = '%1' where id='%2' and applyId='%3'" )
			.arg(time).arg(id).arg(applyid);
	}

	std::cout << sql.toStdString() << std::endl;
	bool b = m_query->exec(sql);
	if (b)
	{
		return m_query->numRowsAffected()>0;
	}
	else
	{
		std::cout << "addApply error" << m_query->lastError().text().toStdString() << std::endl;
	}
	return false;
}

bool MysqlMgr::addFriends(int id, int friendsId)
{
	QString sql = QString("insert into friends values('%1','%2')")
		.arg(id).arg(friendsId);
	std::cout << sql.toStdString() << std::endl;
	if (!m_query->exec(sql))
	{
		std::cout << "addFriends error" << m_query->lastError().text().toStdString() << std::endl;
	}
	return m_query->numRowsAffected() > 0;
}

bool MysqlMgr::loadApplys(int id,QList<Apply>&list)
{
	QString sql = QString("select * from applys where id='%1'").arg(id);
	std::cout << sql.toStdString() << std::endl;
	m_query->exec(sql);
	while (m_query->next())
	{
		int applyId = m_query->value(1).toInt();
		QString applyName = m_query->value(2).toString();
		QString applyTime = m_query->value(3).toString();
		list.append(Apply{ applyId,applyName,applyTime });
	}
	sql = QString("delete from applys where id='%1'").arg(id);
	m_query->exec(sql);
	return true;
}

bool MysqlMgr::loadFriends(int id, QList<PLAYER>& list)
{
	QString sql = QString("select uid,name from player,friends where id='%1' and uid = fid").arg(id);
	std::cout << sql.toStdString() << std::endl;
	m_query->exec(sql);
	while (m_query->next())
	{
		int id = m_query->value(0).toInt();
		QString name = m_query->value(1).toString();
		int state = m_query->value(2).toInt();
		list.append(PLAYER{ id,name,"",""});
		std::cout << id << ' ' << name.toStdString() << ' ' << state << std::endl;
	}
	return true;

}

bool MysqlMgr::deleteFriend(int pid, int delId)
{
	QString sql = QString("delete from friends where id = '%1' and fid = '%2'").arg(pid).arg(delId);
	m_query->exec(sql);
	sql = QString("delete from friends where id = '%1' and fid = '%2'").arg(delId).arg(pid);
	m_query->exec(sql);

	return true;
}

//bool MysqlMgr::updateState(int pid,STATES s)
//{
//	QString sql = QString("update player set state = '%1' where uid = '%2'").arg(s).arg(pid);
//	m_query->exec(sql);
//	return false;
//}
