#ifndef NEXTPANEL_H
#define NEXTPANEL_H

#include "RelatObject.h"
#include "Shape.h"
#include "../exception/ExTemp.h"

extern int LTC_SZ;
extern int SHP_SZ;
class GameDlg; // 前置声明

/** 下一形状面板：
 * 生成并为用户展示下一个下落形状。
 */
class NextPanel : public RelatObject {
public:
	
	/**
	 * ================ 构造、析构 ================
	 */
	
	explicit NextPanel(RelatObject * parent = nullptr);
	
	/** 不允许拷贝构造 */
private:
	NextPanel(const NextPanel & that);
public:
	
	~NextPanel();
	
	/**
	 * ================ 拷贝赋值 ================
	 * 不允许拷贝赋值。
	 */
private:
	NextPanel & operator =(const NextPanel & that);
public:
	
	/**
	 * ================ 查 ================
	 */
	
	virtual int xPix() const;
	
	virtual int yPix() const;
	
	virtual int widthPix() const;
	
	virtual int heightPix() const;
	
	/**
	 * ================ 改 ================
	 */
	
	/** 重新生成形状 */
	inline void refresh() {
		if(nullptr != m_shp) delete m_shp;
		m_shp = new Shape(this);
	}
	
	/**
	 * ================ Getter/Setter ================
	 */
	
	/** 查看但不给出形状：
	 * 不会生成新的下一个形状。 */
	inline const Shape & peek() const { return * m_shp; }
	
	inline       Shape & peek()       { return * m_shp; }
	
	inline NextPanel & setClient(GameDlg & gd) {
		m_client = & gd;
		return * this;
	}
	
	/** 给出形状：
	 * 1, 将形状交给游戏面板。
	 * 2, 同时生成出一个新的下一个形状。
	 * 3, 返回值必须作为参数被GamePanel::setFaller()接收。 */
	inline Shape & give() {
		if(nullptr == m_shp) nullNextEx(ET);
		Shape & oldShp = * m_shp;
		m_shp = new Shape(this);
		return oldShp;
	}
	
	/**
	 * ================ 异常 ================
	 * 1, 将抛异常封装成函数，可统一异常描述信息，也便于统计本类一共有几种异常、每种抛异常的代码在什么地方。
	 * 2, @param ttl ：异常标题，即抛出异常代码所在的源码文件、函数、行号。调用该函数时传参统一用宏“ET”（详见Exception.h）即可。
	 */
	
	inline void nullNextEx(const QString & ttl) const {
		throw ExTemp<NullPtrEx>(ttl, this, "next shape is null");
	}
	
	/**
	 * ================ 内部成员 ================
	 */
protected:
	
	/**
	 * ~~~~~~~~~~~~ 成员变量 ~~~~~~~~~~~~
	 */
	
	Shape * m_shp; // 下一个形状
	GameDlg * m_client; // 图形界面，仅用于获取图形界面的位置和尺寸
	
	/**
	 * ================ 调试 ================
	 */
#ifndef DEBUG

public:
	
	void print(const int level = 0) const;
	
#endif // DEBUG
	
};

#endif // NEXTPANEL_H








