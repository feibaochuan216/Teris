#include "RelatObject.h"
#include "../exception/ExTemp.h"

/**
 * ================ 构造、析构 ================
 */

RelatObject::RelatObject(RelatObject * parent) : m_parent(parent), m_x(0), m_y(0) {}

RelatObject::RelatObject(const int x, const int y, RelatObject * parent)
	: m_parent(parent), m_x(x), m_y(y) {}

RelatObject::~RelatObject() {}

/**
 * ================ 查 ================
 */

int RelatObject::xPix() const {
	if(nullptr == m_parent) nullParentEx(ET);
	return m_parent->xPix() + m_x * LTC_SZ;
}

int RelatObject::yPix() const {
	if(nullptr == m_parent) nullParentEx(ET);
	return m_parent->yPix() + m_y * LTC_SZ;
}

/**
 * ================ 异常 ================
 */

void RelatObject::nullPtrEx(const QString & ttl) const {
	throw ExTemp<NullPtrEx>(ttl, this);
}

void RelatObject::nullParentEx(const QString & ttl) const {
	throw ExTemp<NullParentEx>(ttl, this);
}

/**
 * ================ 调试 ================
 */
#ifndef DEBUG

/** 打印对象信息：
 * @param level：默认为零；作为其他对象的成员变量时，level + 1。 */
void RelatObject::print(const int level/* = 0*/) const {
	
	// 由子类具体实现
	
}

#endif // DEBUG











