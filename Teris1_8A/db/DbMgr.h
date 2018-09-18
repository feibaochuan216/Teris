#ifndef DBMGR_H
#define DBMGR_H

#include "exception/ExTemp.h"
#include "Usr.h"
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QMap>
#include <memory>
using namespace std;

class DbMgrData;
typedef shared_ptr<DbMgrData> DbMgr;

/** 数据库管理者：
 * 1, 负责与数据库交互。
 * @version 1.8A
 * @date 2018.09.11
 * @author 张良尧 */
class DbMgrData {
public:
	
	/**
	 * ================ 创建 ================
	 */
	
	inline static DbMgr newDbMgr(const QString & usrNm, const QString & pswd) {
		DbMgr res(new DbMgrData(usrNm, pswd));
		if(! res->m_open) res->openFailEx(ET); // 数据库打开失败则抛异常
		return res;
	}
	
	void createDb(); // 创建数据库
	
	void createUsrTable(); // 创建用户表
	
	/**
	 * ================ 析构 ================
	 */
	
	~DbMgrData();
	
	/**
	 * ================ 查 ================
	 */
	
	/** 通过用户名和密码查询用户：
	 * @return ：找到则返回true，否则返回false。 */
	bool queryUsr(const QString & usrNm, const QString & pswd);
	
	/** 查询和显示数据表：
	 * 只有系统管理员才能调用。 */
	void query(); // 
	
	/**
	 * ================ 改 ================
	 */
	
	/** 关闭数据库 */
	inline void close() { m_db.close(); }
	
	/**
	 * ================ 异常 ================
	 * 
	 * 1, 将抛异常封装成函数，可统一异常描述信息，也便于统计本类一共有几种异常、每种抛异常的代码在什么地方。
	 * 2, @param ttl ：异常标题，即抛出异常代码所在的源码文件、函数、行号。调用该函数时传参统一用宏“ET”（详见Exception.h）即可。
	 */
	
	/** 数据库打开失败 */
	inline void openFailEx(const QString & ttl) {
		throw ExTemp<DbEx>(ttl, this);
	}
	
	/**
	 * ================ Getter/Setter ================
	 */
	
	inline const Usr & usr() const { return m_usr; }
	
	inline       Usr & usr()       { return m_usr; }
	
	/**
	 * ================ 内部成员 ================
	 */
protected:
	
	/**
	 * ~~~~~~~~~~~~ 成员变量 ~~~~~~~~~~~~
	 */
	
	QSqlDatabase m_db; // 数据库
	bool m_open; // 数据库是否是打开状态
	Usr m_usr; // 当前用户，负责用户信息的保存及游戏存档的解析、反解析
	QSqlQueryModel m_model; // 查询结果的模型
	/** 数据库Usr表中load字段所使用的分隔符 */
	static QString m_kvs; // key-value separator，键值对之间的分隔符
	static QString m_aes; // array element separator，存档数组元素分隔符
	
	/**
	 * ~~~~~~~~~~~~ 构造 ~~~~~~~~~~~~
	 */
	
	DbMgrData(const QString & usrNm, const QString & pswd);
};

#endif // DBMGR_H








