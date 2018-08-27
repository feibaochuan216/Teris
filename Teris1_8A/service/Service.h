#ifndef SERVICE_H
#define SERVICE_H

/** 处理业务逻辑类的基类：
 * 1, 抽象类。
 * 2, 拷贝构造、赋值拷贝都是缺省的浅拷贝。
 * @version 1.7B
 * @date 2018.07.27
 * @author 张良尧 */

#include "../config.h"
#include "../lib/Tools.h"

extern int LTC_SZ;

class Service {
public:
	
	/**
	 * ================ 构造、析构 ================
	 */
	
	Service(Service * parent);
	
	/** 指定坐标：
	 * 创建随机格子时需要。 */
	Service(const int x, const int y, Service * parent);
	
	virtual ~Service() = 0;
	
	/**
	 * ================ 查 ================
	 */
	
	/** 像素坐标 */
	virtual int xPix() const;
	
	virtual int yPix() const;
	
	virtual int widthPix() const = 0;
	
	virtual int heightPix() const = 0;
	
	/**
	 * ================ 改 ================
	 */
	
	inline Service & setPos(const int x, const int y) {
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
	
	inline Service * parent() const { return m_parent; }
	
	inline Service & setParent(Service * parent) {
		m_parent = parent;
		return * this;
	}
	
	inline int x() const { return m_x; }
	
	inline Service & set_x(const int x) {
		m_x = x; 
		return * this;
	}
	
	inline int y() const { return m_y; }
	
	inline Service & set_y(const int y) {
		m_y = y; 
		return * this;
	}
	
	/**
	 * ================ 仅内部使用 ================
	 */
protected:
	
	/**
	 * ~~~~~~~~~~~~ 成员函数 ~~~~~~~~~~~~
	 */

	Service * m_parent;
	int m_x;
	int m_y;
	
};

#endif // SERVICE_H









