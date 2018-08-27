#include "Lattice.h"
#include "../exception/NullParentException.h"

/**
 * ================ 构造、析构 ================
 */

Lattice::Lattice(Service * parent) : Service(parent) {}

Lattice::Lattice(int x, int y, Service * parent) : Service(x, y, parent) {}

Lattice::Lattice(const Lattice & that, Service * parent)
	: Service(that.m_x, that.m_y, parent) {}

Lattice::~Lattice() {}

/**
 * ================ 相等判断 ================
 */

bool operator ==(const Lattice & ltc1, const Lattice & ltc2) {
	if(& ltc1 == & ltc2) return true;
	if(ltc1.m_parent == ltc2.m_parent
	   && ltc1.m_x == ltc2.m_x && ltc1.m_y == ltc2.m_y) {
		return true;
	}
	return false;
}

/**
 * ================ 查 ================
 */

int Lattice::widthPix() const {
	return LTC_SZ;
}

int Lattice::heightPix() const {
	return LTC_SZ;
}

int Lattice::xig() const {
	if(nullptr == m_parent) {
		throw NullParentException(EX_TTL, this);
	}
	return m_parent->x() + m_x;
}

int Lattice::yig() const {
	if(nullptr == m_parent) {
		throw NullParentException(EX_TTL, this);
	}
	return m_parent->y() + m_y;
}

/**
 * ================ 调试 ================
 */
#ifndef DEBUG

void Lattice::print(const int level/* = 0*/) const {
	indent(level);
	cout << "Lattice: " << this << " = {parent: " << m_parent << ", x = " << m_x << ", y = " << m_y << ", xPix = " << xPix() << ", yPix = " << yPix() << '}' << endl;
}

#endif // DEBUG














