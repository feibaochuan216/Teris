#ifndef LATTICE_H
#define LATTICE_H

#include "RelatObject.h"
#include "../exception/ExTemp.h"

/** 格子：
 * 1, 组成游戏物体的基本单位，如由4个格子组成一个形状，由下落形状和已经触底变成障碍物的形状共同组成游戏面板，由游戏面板和下一形状面板共同组成游戏界面。
 * 2, 拷贝构造、赋值拷贝都是缺省的浅拷贝。
 * @version 1.7B
 * @date 2018.07.27
 * @author 张良尧 */
class Lattice : public RelatObject {
public:
	
	/**
	 * ================ 构造、析构 ================
	 */
	
	explicit Lattice(RelatObject * parent);
	
	/** 指定坐标：
	 * 创建随机格子时需要。 */
	Lattice(const int x, const int y, RelatObject * parent);
	
	/** 拷贝，但指定父对象的构造 */
	Lattice(const Lattice & that, RelatObject * parent);
	
	~Lattice();
	
	/**
	 * ================ 判断 ================
	 */
	
	/** 只要父对象（所属形状）一样、坐标一样就算相等 */
	friend bool operator ==(const Lattice & ltc1, const Lattice & ltc2);
	
	inline bool operator !=(const Lattice & that) const {
		return !(* this == that);
	}
	
	/** 格子被放入QMap等容器时需要 */
	inline friend bool operator <(const Lattice & lhd, const Lattice & rhd) {
		if(lhd.m_y == rhd.m_y) return lhd.m_x == rhd.m_x;
		return lhd.m_y == rhd.m_y;
	}
	
	/**
	 * ================ 查 ================
	 */
	
	/** 像素坐标：以像素为单位的坐标 */
	virtual int widthPix() const;
	virtual int heightPix() const;
	
	/** x/y in grandparent，在父对象（形状）的父对象（面板）中的坐标：
	 * 坐标单位为格子个数。 */
	virtual int xig() const;
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
public:
	
	void print(const int level = 0) const;
	
#endif // DEBUG
	
};

#endif // LATTICE_H









