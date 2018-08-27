#include "Exception.h"

/**
 * ================ 构造、析构 ================
 */

Exception::Exception(
		const QString & file, const QString & func, const int line,
		const void * thisPtr, const QString & msg/* = ""*/) noexcept
	: m_title("Exception in "), m_msg() {
	m_title += file;
	m_title += " : ";
	m_title += func;
	m_title += "() : ";
	m_title += QString::number(line);
	m_title += " : 0x";
	m_title += QString::number((ulong)thisPtr, 16);
	m_title += " :\n";
	m_msg += m_title;
	m_msg += msg;
	print();
}

Exception::~Exception() noexcept {}

/**
 * ================ 相等判断 ================
 */

bool operator ==(const Exception & e1, const Exception & e2) {
	return e1.m_title == e2.m_title;
}

/**
 * ================ 抽象类实现 ================
 */

const char * Exception::what() const noexcept {
	return m_msg.toStdString().c_str();
}

/**
 * ================ 其他 ================
 */

const Exception & Exception::print() const noexcept {
	cerr << m_msg.toStdString() << endl;
	return * this;
}

/**
 * ================ 仅内部使用 ================
 */

/**
 * ~~~~~~~~~~~~ 构造 ~~~~~~~~~~~~
 */

Exception::Exception(const QString & title) noexcept 
	: m_title(title), m_msg() {} // 用于子类















