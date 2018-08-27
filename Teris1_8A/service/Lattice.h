#ifndef LATTICE_H
#define LATTICE_H

/** 格子：
 * 1, 拷贝构造、赋值拷贝都是缺省的浅拷贝。
 * @version 1.7B
 * @date 2018.07.27
 * @author 张良尧 */

#include "Service.h"

class Lattice : public Service {
public:
	
	/**
	 * ================ 构造、析构 ================
	 */
	
	Lattice(Service * parent);
	
	/** 指定坐标：
	 * 创建随机格子时需要。 */
	Lattice(const int x, const int y, Service * parent);
	
	/** 拷贝，但指定父对象的构造 */
	Lattice(const Lattice & that, Service * parent);
	
	~Lattice();
	
	/**
	 * ================ 相等判断 ================
	 * 只要父对象一样，坐标一样就算相等。
	 */
	
	friend bool operator ==(const Lattice & ltc1, const Lattice & ltc2);
	
	inline bool operator !=(const Lattice & that) const {
		return !(* this == that);
	}
	
	/**
	 * ================ 查 ================
	 */
	
	/** 像素坐标 */
	virtual int widthPix() const;
	
	virtual int heightPix() const;
	
	/** x in grandparent，在父类的父类中的坐标：
	 * 以格子个数为单位的坐标。 */
	virtual int xig() const;
	
	/** y in grandparent */
	virtual int yig() const;
	
	/**
	 * ================ 改 ================
	 */
	
	/** 移动：
	 * 1, @param dx：横向移动的距离；为正表示向右移，为负表示向左移。
	 * 2, @param dy：纵向移动的距离；为正表示向下移，为负表示向上移。 */
	inline Lattice & move(const int dx, const int dy = 0) {
		m_x += dx;
		m_y += dy;
		return * this;
	}
	
	/** 移动到：
	 * @param x, y：移动到指定的坐标。 */
	inline Lattice & move2(const int x, const int y) {
		m_x = x;
		m_y = y;
		return * this;
	}
	
	/**
	 * ================ 调试 ================
	 */
#ifndef DEBUG
	
	/** 打印对象信息：
	 * @param level：默认为零；作为其他对象的成员变量时，level + 1。 */
	void print(const int level = 0) const;
	
#endif // DEBUG
	
};

#endif // LATTICE_H









