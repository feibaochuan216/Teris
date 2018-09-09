#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <QString>
#include <iostream>
using namespace std;

#define ET QString(QString(__FILE__) + ": " + QString(__func__) + "() line " + QString::number(__LINE__) + ": ")
// Exception Message Titles，宏定义，异常信息的标题，显示报异常的代码所在文件、函数和行号

/** 所有异常的基类：
 * @version 1.7B
 * @date 2018.07.29
 * @author 张良尧 */
class Exception : exception {
public:
	/**
	 * ================ 构造、析构 ================
	 */
	
	/** 1, @param ttl：标题，即抛出异常代码在哪个源码文件、哪个函数、哪个行号。
	 * 2, @param thisPtr ：抛出异常的对象的地址。
	 * 3, @param msg ：异常描述信息。 */
	Exception(
			const QString & ttl, const void * thisPtr, const QString & msg = "") noexcept;
	
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
	 * 用于子类。
	 */
	
	Exception(const QString & title) noexcept;
};

#endif // EXCEPTION_H
