#ifndef USR_H
#define USR_H

#include "../service/GamePanel.h"
#include <QString>
#include <QMap>

class UsrData;
typedef shared_ptr<UsrData> Usr; // UsrData的智能指针

/** 用户：
 * 1, 用于在内存保存、管理从数据库读取的用户数据，并将修改后的用户数据保存到数据库。
 * 2, 用户的游戏存档数据由本类负责解析、反解析：
 * . 2.1, 游戏存档在数据库中以键值对的字符串的形式保存，格式为：
 * "关数:本关的分数,下落形状x坐标:下落形状y坐标,
 * fallerSz:下落形状格子数,
 * 格子1的x坐标:格子1的y坐标,格子2的x坐标:格子2的y坐标, ...
 * 
 * "
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
	 * ================ 游戏存档解析 ================
	 */
	
	static QMap<GamePanel *, Shape *> anlys(const QString & load);
	
	static QString invsAnlys(const QMap<GamePanel *, Shape *> & map);
	
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
	
	inline const QMap<GamePanel *, Shape *> load() const { return m_load; }
	
	inline       QMap<GamePanel *, Shape *> load()       { return m_load; }
	
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
	QMap<GamePanel *, Shape *> m_load;
	
	/**
	 * ~~~~~~~~~~~~ 内部构造 ~~~~~~~~~~~~
	 */
	
	UsrData();
	
	UsrData(const QString & usrNm, UsrType type, qulonglong totalScore, const QString & load);
	
	
	// 未完成
	
	
};

#endif // USR_H


