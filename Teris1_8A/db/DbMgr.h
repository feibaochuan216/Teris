#ifndef DBMGR_H
#define DBMGR_H

#include "exception/ExTemp.h"
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QMap>
#include <memory>
using namespace std;

class DbMgrData;
typedef shared_ptr<DbMgrData> DbMgr;

class DbMgrData {
public:
	
	/**
	 * ================ 内部枚举 ================
	 */
	
	enum UsrType { // 用户类别
		NotExist, // 不存在的用户
		NormalUsr, // 普通用户
		VipUsr, // VIP用户
		MgrUsr // 系统管理员
	};
	
	/**
	 * ================ 创建 ================
	 */
	
	inline static DbMgr newDbMgr() {
		DbMgr res(new DbMgrData);
		if(! res->m_open) res->openFailEx(ET);
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
	
	void query(); // 查询和显示数据表
	
	/**
	 * ================ 改 ================
	 */
	
	/** 关闭数据库 */
	inline void close() { m_db.close(); }
	
	/**
	 * ================ 游戏存档解析 ================
	 */
	
	static QMap<int, qulonglong> analysisLoad(const QString & str);
	// 将数据库中读取的数据解析成存档
	
	static QString load2Str(const QMap<int, qulonglong> & map);
	// 将存档反解析为保存到数据库的字符串
	
	/**
	 * ================ 异常 ================
	 * 
	 * 1, 将抛异常封装成函数，可统一异常描述信息，也便于统计本类一共有几种异常、每种抛异常的代码在什么地方。
	 * 2, @param ttl ：异常标题，即抛出异常代码所在的源码文件、函数、行号。调用该函数时传参统一用宏“ET”（详见Exception.h）即可。
	 */
	
	inline static void analysisFailEx(const QString & ttl, const QString & str) {
		throw ExTemp<DbEx>(
					ttl, nullptr, "string analysised: " + str);
	}
	
	inline void openFailEx(const QString & ttl) {
		throw ExTemp<DbEx>(ttl, this);
	}
	
	/**
	 * ================ Getter/Setter ================
	 */
	
	UsrType curUsrType() const { return m_curUsrType; }
	
	/**
	 * ================ 内部成员 ================
	 */
protected:
	
	/**
	 * ~~~~~~~~~~~~ 成员变量 ~~~~~~~~~~~~
	 */
	
	QSqlDatabase m_db; // 数据库
	bool m_open; // 数据库是否是打开状态
	UsrType m_curUsrType; // 当前用户的类型
	QSqlQueryModel m_model; // 查询结果的模型
	/** 数据库存档中的分隔符 */
	static QString m_kvs; // key-value separator，键值对之间的分隔符
	static QString m_aes; // array element separator，存档数组元素分隔符
	
	/**
	 * ~~~~~~~~~~~~ 构造 ~~~~~~~~~~~~
	 */
	
	DbMgrData();
};

#endif // DBMGR_H








