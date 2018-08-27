#include "Service.h"
#include "../exception/NullParentException.h"

/**
 * ================ 构造、析构 ================
 */

Service::Service(Service * parent) : m_parent(parent), m_x(0), m_y(0) {}

Service::Service(const int x, const int y, Service * parent)
	: m_parent(parent), m_x(x), m_y(y) {}

Service::~Service() {}

/**
 * ================ 查 ================
 */

int Service::xPix() const {
	if(nullptr == m_parent) {
		throw NullParentException(EX_TTL, this);
	}
	return m_parent->xPix() + m_x * LTC_SZ;
}

int Service::yPix() const {
	if(nullptr == m_parent) {
		throw NullParentException(EX_TTL, this);
	}
	return m_parent->yPix() + m_y * LTC_SZ;
}














