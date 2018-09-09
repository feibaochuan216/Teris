/** 1, 不能在构造中抛NullParentException，因为多数情况下parent可能是构造之后再指定的。
 * 2, 每增加一种需要释放的成员变量时，除在析构中要加释放语句外，拷贝构造抛NullPtrException前可能也需要释放。 */

#include "Shape.h"
#include "NextPanel.h"
#include "../lib/Tools.h"
#include <QDebug>
#include <iostream>
using namespace std;

extern int SHP_SZ; // 配置文件中要求的形状默认大小
extern const bool FIXED_SZ; // config.cpp，下落形状是否是固定大小

/**
 * ================ 公有成员变量 ================
 */


/**
 * ================ 构造、析构 ================
 */

Shape::Shape(RelatObject * parent)
	: RelatObject(parent), m_w(0), m_h(0), m_cap(SHP_SZ) {
	if(! FIXED_SZ) { // 下落形状不是固定大小，取值在1～cap或1～SHP_SZ
		m_cap = ::rand() % SHP_SZ + 1;
	}
	Lattice * ltc = new Lattice(::rand() % m_cap, 0, this); // 随机第1个格子
	m_ls += ltc;
	for(int i = 1; i < m_cap; ++i) { // 循环m_cap - 1次
		m_ls += randLtcClose2Exist(new Lattice(this));
	} // 循环m_cap - 1次
	updAlign(updSz());
}

Shape::Shape(const int w, const int h, RelatObject * parent)
	: RelatObject(parent), m_w(0), m_h(0), m_cap(w * h), m_ls() {
	// 形状的实际宽高不一定是w、h，由updXxx()函数计算宽高
	Lattice * ltc = nullptr;
	for(int i = 0; i < h; ++i) {
		for(int j = 0; j < w; ++j) {
			int flg = ::rand() % 100;
			if(flg <= OBS_DENSITY) { // 有 OBS_DENSITY% 的概率生成格子
				if(nullptr == ltc) ltc = new Lattice(j, i, this);
				else ltc->setPos(j, i);
				// ltc非nullptr说明上一轮循环生成的格子没加到形状里
				if(! isRowFull(ltc, w)) { // 新生成的格子不能让一行被填满
					m_ls += ltc;
					ltc = nullptr;
				}
			} // 按概率，本位置应该生成格子
		} // 遍历行
	} // 遍历列
	if(nullptr != ltc) delete ltc; // 没添加到形状里的格子
	updAlign(updSz());
}

Shape::Shape(const Shape & that)
	: RelatObject(that.m_x, that.m_y, that.m_parent),
	m_w(that.m_w), m_h(that.m_h), m_cap(that.m_cap), m_ls() {
	bool flg = false; // 标记：that中是否有NULL格子
	foreach(const Lattice * thatLtc, that.m_ls) {
		if(nullptr == thatLtc) flg = true;
		else {
			Lattice * newLtc = new Lattice(* thatLtc, this);
			m_ls += newLtc;
		}
	}
	if(flg) updAlign(updSz());
}

Shape::Shape(const Shape & that, RelatObject * parent)
	: RelatObject(that.m_x, that.m_y, parent),
	m_w(that.m_w), m_h(that.m_h), m_cap(that.m_cap), m_ls() {
	bool flg = false; // 标记：that中是否有NULL格子
	foreach(const Lattice * thatLtc, that.m_ls) {
		if(nullptr == thatLtc) flg = true;
		else {
			Lattice * newLtc = new Lattice(* thatLtc, this);
			m_ls += newLtc;
		}
	}
	if(flg) updAlign(updSz());
}

Shape::~Shape() {
	if(! m_ls.isEmpty()) {
		QMutableLinkedListIterator<Lattice *> it(m_ls);
		while(it.hasNext()) {
			Lattice * ltc = it.next();
			if(nullptr != ltc) delete ltc;
			it.remove();
		}
	} // if empty
}

/**
 * ================ 拷贝赋值 ================
 */

Shape & Shape::operator =(const Shape & that) {
	if(& that == this) return * this;
	m_parent = that.m_parent;
	m_x = that.m_x;
	m_y = that.m_y;
	m_w = that.m_w;
	m_h = that.m_h;
	m_cap = that.m_cap;
	/* 尽量减少内存的重复分配和销毁，尽量用原有的格子来保存that的格子的值 */
	if(m_ls.size() > that.m_ls.size()) { // this的格子数量 > that
		QMutableLinkedListIterator<Lattice *> thisIt(m_ls);
		foreach(const Lattice * thatLtc, that.m_ls) {
			if(nullptr == thatLtc) nullMbrLtcEx(ET);
			Lattice * thisLtc = thisIt.next();
			if(nullptr == thisLtc) nullMbrLtcEx(ET);
			* thisLtc = * thatLtc; // 格子赋值
			thisLtc->setParent(this);
		} // 遍历that
		while(thisIt.hasNext()) { // this多余的格子删掉
			Lattice * ltc = nullptr;
			if(nullptr != (ltc = thisIt.next())) delete ltc;
			thisIt.remove();
		}
	} else { // this的格子数量 ≤ that
		QLinkedListIterator<Lattice *> thatIt(that.m_ls);
		foreach(Lattice * thisLtc, m_ls) {
			if(nullptr == thisLtc) nullMbrLtcEx(ET);
			Lattice * thatLtc = thatIt.next();
			if(nullptr == thatLtc) nullMbrLtcEx(ET);
			* thisLtc = * thatLtc; // 格子赋值
			thisLtc->setParent(this);
		} // 遍历this
		while(thatIt.hasNext()) {
			Lattice * thatLtc = thatIt.next();
			if(nullptr == thatLtc) nullMbrLtcEx(ET);
			m_ls += new Lattice(* thatLtc, this);
		}
	} // this的格子数量 ≤ that
	return * this;
} // operator =()

/**
 * ================ 判断 ================
 */

/**
 * ~~~~~~~~~~~~ 相等判断 ~~~~~~~~~~~~
 */

bool operator ==(const Shape & shp1, const Shape & shp2) {
	if(& shp1 == & shp2) return true;
	if(shp1.m_ls.size() != shp2.m_ls.size()) return false;
	if(shp1.m_w == shp2.m_w && shp1.m_h == shp2.m_h) {
		foreach(const Lattice * ltc1, shp1.m_ls) {
			if(nullptr == ltc1) shp1.nullMbrLtcEx(ET);
			bool flg = false; // 标记：有没有坐标一样的格子
			foreach(const Lattice * ltc2, shp2.m_ls) {
				if(nullptr == ltc2) shp2.nullMbrLtcEx(ET);
				if(ltc1->x() == ltc2->x() && ltc1->y() == ltc2->y()) {
					flg = true;
				}
			} // 遍历shp2
			if(flg) flg = false; // 有坐标一样的格子，继续循环
			else return false;
		} // 遍历shp1
	} // 如果shp1、shp2尺寸一样
	return true;
}

bool Shape::same(const Shape & that) const {
	if(that == * this) return true;
	Shape tmp(* this);
	for(int i = 0; i < 3; ++i) { // 旋转3次
		if(tmp.rotateClock() == that) return true;
	}
	return false;
}

/**
 * ~~~~~~~~~~~~ 重叠判断 ~~~~~~~~~~~~
 */

bool Shape::isOverlap(const Shape & that) const {
	if(0 == m_ls.size() * that.m_ls.size()) return false;
	foreach(const Lattice * thisLtc, m_ls) {
		if(nullptr == thisLtc) nullMbrLtcEx(ET);
		if(that.isOverlap(* thisLtc)) return true;
	}
	return false;
}

/**
 * ================ 查 ================
 */

/** 本形状在【下一形状】面板时，像素坐标的算法会有所不同 */
int Shape::xPix() const {
	if(nullptr == m_parent) nullParentEx(ET);
	if(typeid(* m_parent) == typeid(NextPanel)) {
		return m_parent->xPix() + (m_parent->widthPix() - m_w * LTC_SZ) / 2;
	} else return m_parent->xPix() + m_x * LTC_SZ;
}

/** 同上 */
int Shape::yPix() const {
	if(nullptr == m_parent) nullParentEx(ET);
	if(typeid(* m_parent) == typeid(NextPanel)) {
		return m_parent->yPix() + (m_parent->heightPix() - m_h * LTC_SZ) / 2;
	} else return m_parent->yPix() + m_y * LTC_SZ;
}

/** 像素宽 */
int Shape::widthPix() const {
	return m_w * LTC_SZ;
}

/** 像素高 */
int Shape::heightPix() const {
	return m_h * LTC_SZ;
}

/**
 * ================ 删 ================
 */

int Shape::rmRows(const QVector<int> & rows) {
	if(rows.isEmpty()) return 0;
	int cnt = 0; // 计数：被删格子数量
	QMutableLinkedListIterator<Lattice *> it(m_ls);
	while(it.hasNext()) {
		Lattice * ltc = it.next();
		if(nullptr == ltc) nullMbrLtcEx(ET);
		for(int i = 0; i < rows.size(); ++i) {
			if(ltc->yig() == rows[i]) {
				it.remove();
				delete ltc;
				++cnt;
				break;
			}
		} // 遍历rows
	} // 遍历m_ls
	if(cnt > 0 && ! m_ls.isEmpty()) {
		QPoint offset = updSz();
		updAlign(offset);
		move(-offset.x(), -offset.y());
	}
	return cnt;
}

bool Shape::rm(const int row, const int col) {
	// 参数的合法性应在游戏面板中已判断
	bool flg = false; // 标记：是否有格子被删除
	QMutableLinkedListIterator<Lattice *> it(m_ls);
	while(it.hasNext()) {
		Lattice * ltc = it.next();
		if(nullptr == ltc) nullMbrLtcEx(ET);
		if(row == ltc->yig() && col == ltc->xig()) {
			delete ltc;
			ltc = nullptr;
			it.remove();
			flg = true;
			break;
		}
	} // 遍历本形状
	return flg;
}

/**
 * ================ 改 ================
 */

QPoint Shape::updSz() {
	int max_x = 0, max_y = 0, min_x = m_cap, min_y = m_cap;
	// 所有格子中的最大、最小坐标值
	foreach(const Lattice * ltc, m_ls) {
		if(nullptr == ltc) nullMbrLtcEx(ET);
		if(ltc->x() > max_x) max_x = ltc->x();
		if(ltc->x() < min_x) min_x = ltc->x();
		if(ltc->y() > max_y) max_y = ltc->y();
		if(ltc->y() < min_y) min_y = ltc->y();	
	}
	m_w = max_x - min_x + 1;
	m_h = max_y - min_y + 1;
	QPoint pt(min_x, min_y);
	return pt;
}

Shape & Shape::updAlign(const QPoint & offset) {
	if (0 == offset.x() && 0 == offset.y()) return * this;
	foreach(Lattice * ltc, m_ls) {
		if(nullptr == ltc) nullMbrLtcEx(ET);
		ltc->set_x(ltc->x() - offset.x());
		ltc->set_y(ltc->y() - offset.y());
	}
	return * this;
}

Shape & Shape::rotateClock() {
	foreach(Lattice * ltc, m_ls) {
		if(nullptr == ltc) nullMbrLtcEx(ET);
		int tmp = ltc->x();
		ltc->set_x(m_cap - ltc->y() - 1);
		ltc->set_y(tmp);
	}
	int ow = m_w; // old width
	int oh = m_h; // old height
	updAlign(updSz());
	return move((ow - m_w) / 2, oh - m_h);
}

Shape & Shape::rotateAnti() {
	foreach(Lattice * ltc, m_ls) {
		if(nullptr == ltc) nullMbrLtcEx(ET);
		int tmp = ltc->x();
		ltc->set_x(ltc->y());
		ltc->set_y(m_cap - tmp - 1);
	}
	int ow = m_w; // old width
	int oh = m_h; // old height
	updAlign(updSz());
	return move((ow - m_w) / 2, oh - m_h);
}

/**
 * ================ 仅内部使用 ================
 */

/**
 * ~~~~~~~~~~~~ 随机格子 ~~~~~~~~~~~~
 * 这几个方法的组合是创建随机格子的效率最高的。
 */

/** 随机创建非第1个的格子：
 * <1, 从现有格子中随机取出一个。
 * 2, 在这个现有格子的上、下、左、右的其中一个方向随机生成一个新格子。
 * 3, 如果新格子出了形状的范围，或者与已有格子重叠，则重新生成。*/
Lattice * Shape::randLtcClose2Exist(Lattice * ltc) {
	/* <1 */
	int index = ::rand() % m_ls.size(); // 随机取出的格子的下标，0 ≤ < m_ls.size()
	QLinkedListIterator<Lattice *> it(m_ls);
	for(int i = 0; i < index; ++i) it.next();
	Lattice * exist = it.next(); // 随机取出的已有格子
	if(nullptr == exist) nullMbrLtcEx(ET);
	do {
		int dir = ::rand() % 4; // 从上下左右中随机选一个方向
		switch(dir) {
		case 0: // 上
			ltc->setPos(exist->x(), exist->y() - 1);
			break;
		case 1: // 右
			ltc->setPos(exist->x() + 1, exist->y());
			break;
		case 2: // 下
			ltc->setPos(exist->x(), exist->y() + 1);
			break;
		default: // 左
			ltc->setPos(exist->x() - 1, exist->y());
		}
	} while(isOverlap(* ltc));
	return ltc;
} // randLtcAfter1st()

/** 让指定的格子在指定范围内位置随机：
 * 1, @param w：宽，必须 > 0。
 * 2, @param h：高，必须 > 0。
 * 3, @param ltc：其m_parent必须是this。 */
Lattice * Shape::randLtc(Lattice * ltc, const int w, const int h) {
	ltc->set_x(::rand() % w);
	ltc->set_y(::rand() % h);
	return ltc;
}

/**
 * ~~~~~~~~~~~~ 其他 ~~~~~~~~~~~~
 */

/** 是否满行：
 * 1, 用于生成障碍物的初始形状，如果新加的格子让一行满了，是不符合要求的，需要重新生成。
 * 2, @param ltc：新加的格子，不能为nullptr。
 * 3, @param width：游戏面板的宽度。 */
bool Shape::isRowFull(const Lattice * ltc, const int gpWidth) {
	int cnt = 0; // 计数：指定行有几个格子
	foreach(const Lattice * exist, m_ls) {
		if(nullptr == exist) nullMbrLtcEx(ET);
		if(ltc->y() == exist->y()) ++cnt;
	}
	if(gpWidth == cnt) return true;
	return false;
}

/**
 * ================ 调试 ================
 */
#ifndef DEBUG

void Shape::print(const int level/* = 0*/) const {
	indent(level);
	cout << "Shape(" << this << "):{parent: " << m_parent
		 << ", x: " << m_x << ", y: " << m_y 
		 << ", w: " << m_w << ", h: " << m_h<< ", cap: " << m_cap
		 << ',' << endl;
	indent(level + 1);
	cout << "xPix: " << xPix() << ", yPix: " << yPix() 
		 << ", wPix: " << widthPix() << ", hPix: " << heightPix() 
		  << ", ls: [" << endl;
	foreach(Lattice * ltc, m_ls) {
		ltc->print(level + 2);
	}
	indent(level);
	cout << "]}" << endl;
}

#endif // DEBUG














