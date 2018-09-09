#ifndef SERVICE_H
#define SERVICE_H

#include "../config.h"
#include "../lib/Tools.h"

extern int LTC_SZ;

/** Relational Object，关联性物体：
 * 1, 和其他物体有上下级的关系，如格子的上级是形状，形状的上级是面板。
 * 2, 上级物体在释放时负责释放它的下级。如面板释放时会释放形状，形状释放时会释放格子，这样可以保证所有相关物体都能得到释放。
 * 3, 抽象类。
 * 4, 拷贝构造、赋值拷贝都是缺省的浅拷贝。
 * @version 1.8A
 * @date 2018.09.08
 * @author 张良尧 */
class RelatObject {
public:
	
	/**
	 * ================ 构造、析构 ================
	 */
	
	RelatObject(RelatObject * parent);
	
	/** 指定坐标：
	 * 创建随机格子时需要。 */
	RelatObject(const int x, const int y, RelatObject * parent);
	
	virtual ~RelatObject() = 0;
	
	/**
	 * ================ 查 ================
	 */
	
	/** 获取像素坐标 */
	virtual int xPix() const;
	
	virtual int yPix() const;
	
	virtual int widthPix() const = 0;
	
	virtual int heightPix() const = 0;
	
	/**
	 * ================ 改 ================
	 */
	
	inline RelatObject & setPos(const int x, const int y) {
		m_x = x;
		m_y = y;
		return * this;
	}
	
	/**
	 * ================ 其他 ================
	 */
	
	static int rand();
	
	/**
	 * ================ Getter/Setter ================
	 */
	
	inline RelatObject * parent() const { return m_parent; }
	
	inline RelatObject & setParent(RelatObject * parent) {
		m_parent = parent;
		return * this;
	}
	
	inline int x() const { return m_x; }
	
	inline RelatObject & set_x(const int x) {
		m_x = x; 
		return * this;
	}
	
	inline int y() const { return m_y; }
	
	inline RelatObject & set_y(const int y) {
		m_y = y; 
		return * this;
	}
	
	/**
	 * ================ 异常 ================
	 * 1, 将抛异常封装成函数，可统一异常描述信息，也便于统计本类一共有几种异常、每种抛异常的代码在什么地方。
	 * 2, @param ttl ：异常标题，即抛出异常代码所在的源码文件、函数、行号。调用该函数时传参统一用宏“ET”（详见Exception.h）即可。
	 */
	
	/** 空指针异常 */
	virtual void nullPtrEx(const QString & ttl) const;
	
	
	/** 空父物体异常 */
	virtual void nullParentEx(const QString & ttl) const;
	
	/**
	 * ================ 内部成员 ================
	 */
protected:
	
	/**
	 * ~~~~~~~~~~~~ 成员函数 ~~~~~~~~~~~~
	 */

	RelatObject * m_parent;
	int m_x;
	int m_y;
	
	/**
	 * ================ 调试 ================
	 */
#ifndef DEBUG
	
public:
	virtual void print(const int level = 0) const;
	
#endif // DEBUG
};

#endif // SERVICE_H









