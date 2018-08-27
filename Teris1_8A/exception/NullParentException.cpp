#include "NullParentException.h"

/**
 * ================ 构造、析构 ================
 */

NullParentException::NullParentException (
		const QString & file, const QString & func, const int line,
		const void * thisPtr, const QString & msg/* = ""*/) noexcept 
	: NullPtrException("NullParentException in ") {
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

NullParentException::~NullParentException() noexcept {}

/**
 * ================ 仅内部使用 ================
 */

/** 用于子类 */
NullParentException::NullParentException(const QString & title) noexcept
	: NullPtrException(title) {}










