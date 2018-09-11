#include "DbMgr.h"
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
 * ================ 创建 ================
 */

void DbMgrData::createDb() {
	m_db = QSqlDatabase::addDatabase("QSQLITE");
	m_db.setDatabaseName("teris.db"); // 指定数据库文件名
	if(false == m_db.open()) {
		qDebug() << m_db.lastError().text();
	} else {
		m_open = true;
	}
}

/**
 * ================ 析构 ================
 */

DbMgrData::~DbMgrData() {
	if(m_open) m_db.close();
}

void DbMgrData::createUsrTable() {
	QString sql("create table usr ("
				"id int primary key not null,"
				"name text not null,"
				"pswd text not null,"
				"type int not null,"
				"load text);");
		qDebug() << sql;
		QSqlQuery query;
		query.exec(sql);
}

/**
 * ================ 查 ================
 */

bool DbMgrData::queryUsr(const QString & usrNm, const QString & pswd) {
	QString sql("select type from usr where name = '%1' and pswd = '%2'");
	sql = sql.arg(usrNm).arg(pswd);
	
	qDebug() << sql;
	
	QSqlQuery q(sql);
	if(q.next()) {
		m_curUsrType = static_cast<UsrType>(q.value(0).toInt());
		return true;
	} else {
		m_curUsrType = NotExist;
		return false;
	}
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
 * ================ 内部成员 ================
 */

/**
 * ~~~~~~~~~~~~ 构造 ~~~~~~~~~~~~
 */

DbMgrData::DbMgrData() : m_curUsrType(NormalUsr) {
	createDb();
	createUsrTable();
}











