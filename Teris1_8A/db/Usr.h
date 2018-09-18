#ifndef USR_H
#define USR_H

#include "Load.h"
#include "../exception/ExTemp.h"
#include <QString>
#include <QMap>

class UsrData;
typedef shared_ptr<UsrData> Usr; // UsrData的智能指针

/** 用户：
 * 1, 用于在内存保存、管理从数据库读取的用户数据，并将修改后的用户数据保存到数据库。
 * 2, 用户的游戏存档数据由本类负责解析、反解析。
 * @version 1.8A
 * @date 2018.09.18
 * @author 张良尧 */
class UsrData {
public:
	
	/**
	 * ================ 内部枚举 ================
	 */
	
	enum UsrType { // 用户类别
		NotExistUsr, // 不存在的用户
		NormalUsr, // 普通用户
		VipUsr, // VIP用户
		MgrUsr // 系统管理员
	};
	
	enum UsrDbIndx { // 成员变量对应的数据库的字段的下标
		IdIndx,
		NameIndx,
		PswdIndx,
		TypeIndx,
		TotalScoreIndx,
		LoadIndx
	};
	
	/**
	 * ================ 创建 ================
	 */
	
	inline static Usr newUsr(const QString & usrNm, UsrType type, qulonglong totalScore, const QString & load) {
		Usr res(new UsrData(usrNm, type, totalScore, load));
		return res;
	}
	
	/**
	 * ================ 析构 ================
	 */
	
	~UsrData();
	
	/**
	 * ================ Getter/Setter ================
	 */
	
	inline const QString & usrNm() const { return m_usrNm; }
	
	inline void setUsrNm(const QString & usrNm) { m_usrNm = usrNm; }
	
	inline UsrType type() const { return m_type; }
	
	inline void setType(UsrType type) { m_type = type; }
	
	inline qulonglong totalScore() const { return m_totalScore; }
	
	inline void setTotalScore(qulonglong totalScore) {
		m_totalScore = totalScore;
	}
	
	inline const Load & load() const { return m_load; }
	
	inline       Load & load()       { return m_load; }
	
	/**
	 * ================ 内部成员 ================
	 */
protected:
	
	/**
	 * ~~~~~~~~~~~~ 成员变量 ~~~~~~~~~~~~
	 */
	
	QString m_usrNm;
	UsrType m_type;
	qulonglong m_totalScore;
	Load m_load;
	
	/**
	 * ~~~~~~~~~~~~ 内部构造 ~~~~~~~~~~~~
	 */
	
	UsrData();
	
	UsrData(const QString & usrNm, UsrType type, qulonglong totalScore, const QString & load);
	
	
	// 未完成
	
	
};

#endif // USR_H


