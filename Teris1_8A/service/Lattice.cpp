#include "Lattice.h"
#include <QDebug>

/**
 * ================ 构造、析构 ================
 */

Lattice::Lattice(RelatObject * parent) : RelatObject(parent) {}

Lattice::Lattice(int x, int y, RelatObject * parent) : RelatObject(x, y, parent) {}

Lattice::Lattice(const Lattice & that, RelatObject * parent)
	: RelatObject(that.m_x, that.m_y, parent) {}

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
	if(nullptr == m_parent) nullParentEx(ET);
	return m_parent->x() + m_x;
}

int Lattice::yig() const {
	if(nullptr == m_parent) nullParentEx(ET);
	return m_parent->y() + m_y;
}

/**
 * ================ 其他 ================
 */

Lattice * Lattice::anlys(const QString & load, int * cur, RelatObject * parent) {
	int end = load.indexOf(KEY_VAL_SEP, * cur); // 字符串中代表x坐标的字符串的结尾位置
	
	qDebug() << "loading ltc:" << load.mid(* cur, end);
	
	Lattice * res = new Lattice(parent);
	res->m_x = load.mid(* cur, end).toInt();
	* cur = end + 1; // 字符串中代表y坐标的字符串的起始位置
	end = load.indexOf(ARR_ELMT_SEP, * cur); // 字符串中代表y坐标的字符串的结尾位置
	
	qDebug() << "loading ltc:" << load.mid(* cur, end);
	
	res->m_y = load.mid(* cur, end).toInt();
	* cur = end + 1;
	return res;
}

/**
 * ================ 调试 ================
 */
#ifndef DEBUG

/** 打印对象信息：
 * @param level：默认为零；作为其他对象的成员变量时，level + 1。 */
void Lattice::print(const int level/* = 0*/) const {
	indent(level);
	cout << "Lattice(" << this << "):{parent: " << m_parent << ", x: " << m_x << ", y: " << m_y << ", xPix: " << xPix() << ", yPix: " << yPix() << '}' << endl;
}

#endif // DEBUG














