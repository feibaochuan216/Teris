#include "OutRangeGeomException.h"

/**
 * ================ 构造、析构 ================
 */

OutRangeGeomException::OutRangeGeomException (
		const QString & file, const QString & func, const int line,
		const void * thisPtr, const QString & msg/* = ""*/) noexcept
	: GeomException("OutRangeGeomException in ") {
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

OutRangeGeomException::~OutRangeGeomException() noexcept {}

/**
 * ================ 仅内部使用 ================
 */

/** 用于子类 */
OutRangeGeomException::OutRangeGeomException(const QString & title) noexcept : GeomException(title) {}









