#ifndef USR_H
#define USR_H

#include <QString>

class Usr {
public:
	
	/**
	 * ================ 内部枚举 ================
	 */
	
	enum UsrType { // 用户类别
		Normal, // 普通用户
		Vip, // VIP用户
		Mgr // 系统管理员
	};
	
	/**
	 * ================ 构造 ================
	 */
	
	Usr();
	
	Usr(const QString & usrNm, const QString & pswd, UsrType type = Normal);
	
	/**
	 * ================ Getter/Setter ================
	 */
	
	inline const QString & usrNm() const { return m_usrNm; }
	
	inline void setUsrNm(const QString & usrNm) { m_usrNm = usrNm; }
	
	inline const QString & pswd() const { return m_pswd; }
	
	inline void setPswd(const QString & pswd) { m_pswd = pswd; }
	
	inline UsrType type() const { return m_type; }
	
	inline void setType(UsrType type) { m_type = type; }
	
	inline qulonglong totalScore() const { return m_totalScore; }
	
	inline void setTotalScore(qulonglong totalScore) { m_totalScore = totalScore; }
	
	/**
	 * ================ 内部成员 ================
	 */
protected:
	
	/**
	 * ~~~~~~~~~~~~ 成员变量 ~~~~~~~~~~~~
	 */
	
	QString m_usrNm; // 用户名
	QString m_pswd; // 密码
	UsrType m_type; // 用户类别
	//m_load; // 存档
	qulonglong m_totalScore; // 总分数
};

#endif // USR_H
