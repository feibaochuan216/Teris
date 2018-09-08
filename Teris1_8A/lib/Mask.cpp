#include "Mask.h"
using namespace std;

/**
 * ================ 构造、析构 ================
 */
Mask::Mask() : m_data(new uchar), m_sz(0) {}

Mask::Mask(const int sz) : m_data(nullptr), m_sz(0) {
	int cap; // 数组长度，判断后赋初值
	if(sz > 0) {
		cap = mkCap(sz);
		m_data = new uchar[cap];
		this->m_sz = sz;
	} else {
		cap = 1;
		m_data = new uchar;
	}
	for(int i = 0; i < cap; ++i) m_data[i] = 0;
}

Mask::Mask(const Mask & that) : m_data(new uchar[mkCap(that.m_sz)]), m_sz(that.m_sz) {
	for(int i = 0; i < m_sz; ++i) m_data[i] = that.m_data[i];
}

Mask::~Mask() {
	delete m_data;
}

/**
 * ================ 赋值拷贝 ================
 */
Mask & Mask::operator =(const Mask & that) {
	if(& that == this) return * this;
	if(needRecap(that.m_sz)) {
		delete m_data;
		m_data = new uchar[mkCap(that.m_sz)];
	}
	for(int i = 0; i < that.m_sz; ++i) m_data[i] = that.m_data[i];
	m_sz = that.m_sz;
	return * this;
}

/**
 * ================ 判断 ================
 */
bool operator ==(const Mask & m1, const Mask & m2) {
	if(& m1 == & m2) return true;
	if(m1.m_sz != m2.m_sz) return false;
	for(int i = 0; i < m1.m_sz; ++i) {
		if(m1.m_data[i] != m2.m_data[i]) return false;
	}
	return true;
}

/**
 * ================ 增 ================
 */
Mask & Mask::addBit(const int ds) {
	if(ds < 0) return rmBit(-ds);
	if(0 == ds) return * this;
	int size = m_sz + ds;
	if(needRecap(size)) {
		uchar * newData = new uchar[mkCap(size)];
		for(int i = 0; i < m_sz; ++i) newData[i] = m_data[i];
		delete m_data;
		m_data = newData;
	}
	m_sz = size;
	return * this;
}

/**
 * ================ 删 ================
 */
Mask & Mask::rmBit(const int ds) {
	if(ds < 0) return addBit(-ds);
	if(0 == ds) return * this;
	int size = m_sz - ds;
	if(size <= 0) {
		if(m_sz <= BASE_SZ) return * this;
		/* sz > BASE_SZ */
		uchar * newData = new uchar;
		for(int i = 0; i < BASE_SZ; ++i) newData[i] = m_data[i];
		delete m_data;
		m_data = newData;
		m_sz = BASE_SZ;
	} else { // size > 0
		if(needRecap(size)) {
			uchar * newData = new uchar[mkCap(size)];
			for(int i = 0; i < size; ++i) newData[i] = m_data[i];
			delete m_data;
			m_data = newData;
		}
		m_sz = size;
	}
	return * this;
}

/**
 * ================ 改 ================
 */
Mask & Mask::setVal(const int index, const bool v) {
	int bi = indexOfByte(index); // byte index
	if(v) m_data[bi] |= mkMask(indexOfBit(index));
	else {
		m_data[bi] = ~((~m_data[bi]) | mkMask(indexOfBit(index)));
	}
	return * this;
}

Mask & Mask::fill(const bool v) {
	int cnt = indexOfByte(m_sz); // 一共有cnt个uchar要遍历
	uchar mask;
	if(v) mask = 255;
	else mask = 0;
	for(int i = 0; i < cnt; ++i) m_data[i] |= mask;
	return * this;
}

/**
 * ================ 输入输出重载 ================
 */

ostream & operator <<(ostream & os, const Mask & This) {
	for(int i = 0; i < This.m_sz; ++i) {
		if(0 != i && 0 == i % 4) os << ' ';
		if(This[i]) os << '1';
		else os << '0';
	}
	return os;
}

istream & operator >>(istream & is, Mask & This) {
	string buf;
	is >> buf;
	size_t sz = buf.size();
	for(size_t i = 0, index = 0; i < sz; ++i) {
		if('0' == buf[i]) {
			This.setVal(index, false);
			++index;
		} else if('1' == buf[i]) {
			This.setVal(index, true);
			++index;
		}
	}
	return is;
}

/**
 * ================ 仅内部使用 ================
 */

/**
 * ~~~~~~~~~~~~ 构造 ~~~~~~~~~~~~
 */

/** 构造一个新的指定个数的布尔值集合，并拷贝指定布尔值：
 * @param data ：要拷贝的布尔值。
 * @param len ：data数组的长度。
 * @param sz ：新集合的布尔值个数。 */
Mask::Mask(const uchar * data, int len, int sz) : m_data(new uchar[mkCap(sz)]), m_sz(sz) {
	for(int i = 0; i < len; ++i) {
		m_data[i] = data[i];
	}
}

/**
 * ~~~~~~~~~~~~ 改 ~~~~~~~~~~~~
 */

/** 交换两个对象的数据：
 * 用于替换成临时对象的数据，如果出现错误，数据不会马上丢失。 */
Mask & Mask::swap(Mask & that) {
	uchar * tmp = that.m_data;
	that.m_data = m_data;
	m_data = tmp;
	m_sz = m_sz ^ that.m_sz;
	that.m_sz = m_sz ^ that.m_sz;
	m_sz = m_sz ^ that.m_sz;
	return * this;
}










