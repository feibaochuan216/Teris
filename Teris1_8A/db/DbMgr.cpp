#include "DbMgr.h"
#include "exception/ExTemp.h"
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
#include <QSqlQuery>

/**
 * ================ 静态成员变量 ================
 */

QString DbMgrData::m_kvs = ":";
QString DbMgrData::m_aes = ",";

/**
 * ================ 构造 ================
 */

DbMgrData::DbMgrData() {
	createDb();
	createUsrTable();
}

void DbMgrData::createDb() {
	m_db = QSqlDatabase::addDatabase("QSQLITE");
	m_db.setDatabaseName("teris.db"); // 指定数据库文件名
	if(false == m_db.open()) {
		qDebug() << m_db.lastError().text();
	}
}

void DbMgrData::createUsrTable() {
	QString sql("create table usr ("
				"id int primary key not null,"
				"name text not null,"
				"pswd text not null,"
				"load text);");
		qDebug() << sql;
		QSqlQuery query;
		query.exec(sql);
}

void DbMgrData::query() {
	
	
	
	// 未完成
	
	
	
}



/**
 * ================ 游戏存档解析 ================
 */

QMap<int, qulonglong> DbMgrData::analysisLoad(const QString & str) {
	qDebug() << "解析：" << str;
	QMap<int, qulonglong> map;
	int seek = 0; // 字符串的流指针
	while(seek < str.size()) {
		/* 获取键 */
		int keyIndx = str.indexOf(m_kvs, seek);
		bool success;
		int key = str.mid(seek, keyIndx).toInt(& success);
		if(! success) analysisFailEx(ET, str);
		seek = keyIndx + 1;
		/* 获取值 */
		qulonglong value = 0;
		int valueIndx = str.indexOf(m_aes, seek);
		value = str.mid(seek, valueIndx).toInt(& success);
		// 如果已经解析到字符串的结尾，结尾没有m_aes，valueIndx值为-1，则mid()将会截取到字符串结尾
		if(! success) analysisFailEx(ET, str);
		seek = valueIndx + 1;
		map.insert(key, value);
	}
	return map;
}

QString DbMgrData::load2Str(const QMap<int, qulonglong> & map) {
	QString res;
	QMapIterator<int, qulonglong> it(map);
	while(it.hasNext()) {
		it.next();
		if(! res.isEmpty()) res += m_aes;
		res += QString::number(it.key()) += m_kvs += QString::number(it.value());
	}
	return res;
}

/**
 * ================ 异常 ================
 * 
 * 1, 将抛异常封装成函数，可统一异常描述信息，也便于统计本类一共有几种异常、每种抛异常的代码在什么地方。
 * 2, @param ttl ：异常标题，即抛出异常代码所在的源码文件、函数、行号。调用该函数时传参统一用宏“ET”（详见Exception.h）即可。
 */

void DbMgrData::analysisFailEx(const QString & ttl, const QString & str) {
	throw ExTemp<DbEx>(ttl, nullptr, "string analysised: " + str);
}













