#ifndef EXCEPTION_H
#define EXCEPTION_H

/** 所有异常的基类：
 * @version 1.7B
 * @date 2018.07.29
 * @author 张良尧 */

#include <QString>
#include <iostream>

#define EX_TTL __FILE__, __func__, __LINE__ // Exception Message Titles

using namespace std;

class Exception : exception {
public:
	/**
	 * ================ 构造、析构 ================
	 */
	
	Exception(
			const QString & file, const QString & func, const int line,
			const void * thisPtr, const QString & msg = "") noexcept;
	
	virtual ~Exception() noexcept;
	
	/**
	 * ================ 相等判断 ================
	 */
	
	friend bool operator ==(const Exception & e1, const Exception & e2);
	
	bool operator !=(const Exception & that) const {
		return !(* this == that);
	}
	
	/**
	 * ================ 抽象类实现 ================
	 */
	
	virtual const char * what() const noexcept;
	
	/**
	 * ================ 其他 ================
	 */
	
	virtual const Exception & print() const noexcept;
	
	inline Exception & operator +=(const QString & msg) {
		m_msg += msg;
		return * this;
	}
	
	/**
	 * ================ Getter/Setter ================
	 */
	
	inline const QString & title() const noexcept { return m_title; }
	
	inline const QString msg() const noexcept { return m_msg; }
	
	inline Exception & setMsg(const QString & msg) noexcept {
		m_msg = msg;
		return * this;
	}
	
	/**
	 * ================ 仅内部使用 ================
	 */
protected:
	/**
	 * ~~~~~~~~~~~~ 成员变量 ~~~~~~~~~~~~
	 */
	
	QString m_title; // 异常信息中的标题部分
	QString m_msg; // 异常信息
	
	/**
	 * ~~~~~~~~~~~~ 构造 ~~~~~~~~~~~~
	 */
	
	Exception(const QString & title) noexcept;
	
};

#endif // EXCEPTION_H
