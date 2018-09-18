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

void DbMgrData::createUsrTable() {
	QString sql("create table usr ("
				"id int primary key not null,"
				"name text not null,"
				"pswd text not null,"
				"type int not null,"
				"totalScore int not null,"
				"load text);");
		qDebug() << sql;
		QSqlQuery query;
		query.exec(sql);
}

/**
 * ================ 析构 ================
 */

DbMgrData::~DbMgrData() {
	if(m_open) m_db.close();
}

/**
 * ================ 查 ================
 */

bool DbMgrData::queryUsr(const QString & usrNm, const QString & pswd) {
	QString sql("select * from usr where name = '%1' and pswd = '%2'");
	sql = sql.arg(usrNm).arg(pswd);
	
	qDebug() << sql;
	
	QSqlQuery q(sql);
	if(q.next()) {
		m_usr = UsrData::newUsr(
					q.value(UsrData::NameIndx).toString(),
					static_cast<UsrData::UsrType>(
						q.value(UsrData::TypeIndx).toInt()),
					q.value(
						UsrData::TotalScoreIndx).toULongLong(),
					q.value(UsrData::LoadIndx).toString());
		return true;
	} else {
		return false;
	}
}

void DbMgrData::query() {
	
	
	
	// 未完成
	
	
	
}

/**
 * ================ 内部成员 ================
 */

/**
 * ~~~~~~~~~~~~ 构造 ~~~~~~~~~~~~
 */

DbMgrData::DbMgrData(const QString & usrNm, const QString & pswd) : m_usr(nullptr) {
	createDb();
	createUsrTable();
	if(! queryUsr(usrNm, pswd)) {
		m_usr = nullptr;
	}
}











