#ifndef SHAPE_H
#define SHAPE_H
//test
/** 形状：
 * 1, 拷贝构造和拷贝赋值都是深拷贝。
 * @version 1.7B
 * @date 2018.07.27
 * @author 张良尧 */

#include "RelatObject.h"
#include "Lattice.h"
#include "../exception/ExTemp.h"
#include "../config.h"
#include <QString>
#include <QLinkedList> // 形状的格子集合
#include <QPoint> // 更新尺寸时需要
#include <QVector> // 消行时需要

extern int LTC_SZ;
extern const QString KEY_VAL_SEP;
extern const QString ARR_ELMT_SEP;

class Shape : public RelatObject {
public:
	
	/**
	 * ================ 构造、析构 ================
	 */
	
	/** 生成随机形状：
	 * 1, 游戏配置为固定大小时，形状大小为SHP_SZ。
	 * 2, 游戏配置为变长形状时，容量为1～SHP_SZ的随机数。 */
	explicit Shape(RelatObject * parent);
	
	/** 指定宽高：
	 * 1, 主要用于生成障碍物。
	 * 2, @param w：宽度，必须 > 0 且 ≤ 游戏面板的宽度。
	 * 3, @param h：高度：必须 > 0 且 ≤ OBS_MAX。
	 * 4, @param parent：游戏面板的地址。 */
	Shape(const int w, const int h, RelatObject * parent);
	
	/** 拷贝构造：
	 * 1, 深拷贝。新形状的格子和参数形状的格子完全是两个对象。
	 * 2, 但父对象地址是浅拷贝。 */
	Shape(const Shape & that);
	
	/** 拷贝但指定父对象的构造 */
	Shape(const Shape & that, RelatObject * parent);
	
	~Shape();
	
	/**
	 * ================ 拷贝赋值 ================
	 */
	
	Shape & operator =(const Shape & that);
	
	/**
	 * ================ 判断 ================
	 */
	
	/**
	 * ~~~~~~~~~~~~ 相等判断 ~~~~~~~~~~~~
	 */
	
	/** 无视位置，大小、形状、旋转方向一样：
	 * @param shp1、shp2：必须在同一个面板中。 */
	friend bool operator ==(const Shape & shp1, const Shape & shp2);
	
	inline bool operator !=(const Shape & that) const {
		return !(* this == that);
	}
	
	/** 无视位置和旋转方向，只要大小、形状一样：
	 * @param that：不一定和this在同一个面板中。 */
	bool same(const Shape & that) const;
	
	/**
	 * ~~~~~~~~~~~~ 重叠判断 ~~~~~~~~~~~~
	 */
	
	inline bool isOverlap(const Lattice & ltc) const {
		foreach(const Lattice * mbr, m_ls) {
			if(nullptr == mbr) nullMbrLtcEx(ET);
			if(mbr->xig() == ltc.xig() && mbr->yig() == ltc.yig()) {
				return true;
			}
		}
		return false;
	}
	
	bool isOverlap(const Shape & that) const;
	
	/**
	 * ================ 查 ================
	 */
	
	/** 像素坐标 */
	virtual int xPix() const;
	
	/** 像素坐标 */
	virtual int yPix() const;
	
	/** 像素宽 */
	virtual int widthPix() const;
	
	/** 像素高 */
	virtual int heightPix() const;
	
	/** 格子的个数 */
	inline int size() const { return m_ls.size(); }
	
	/**
	 * ================ 增 ================
	 */
	
	inline Shape & operator +=(Lattice & ltc) {
		m_ls += & ltc;
		ltc.setParent(this);
		return * this;
	}
	
	/**
	 * ================ 删 ================
	 */
	
	/** 消行：
	 * 1, @param rows：要消行的行号。不能为空。
	 * 2, @param col：删第几列的格子：
	 * . 2.1, 如果消行时需要动画效果，可能需要一列一列的删。
	 * . 2.2, < 0 时，则没有动画效果，整行一次性删除。
	 * 3, @return 被删格子的数量，用于判断本形状是否被删空。
	 * 4, 消行后本形状可能会变成一个空形状。 */
	int rmRows(const QVector<int> & rows);
	
	/** 消除格子：
	 * 1, 用于实现有动画效果的消行，格子可能需要一列一列地删，甚至一个一个地删。
	 * 2, @param row：删除第几行的格子；必须 ≥ 0。
	 * 3, @param col：删第几列的格子；必须 ≥ 0 。
	 * 4, @return 是否有格子被删除，用于判断本形状是否被删空。 */
	bool rm(const int row, const int col);
	
	/**
	 * ================ 改 ================
	 */
	
	/** 更新形状的尺寸和位置：
	 * 当形状里的格子发生变化时，可能所有格子不再靠形状的左上角对齐，让它们在形状内靠左上角对齐，而格子在面板中的位置不变。 */
	Shape & updSzNPos();
	
	/** 更新尺寸：
	 * 1, 新增、删除格子，或者旋转等操作后，重新计算形状的宽和高的值。
	 * 2, @return ：返回一个尺寸，如果所有格子没有靠左上对齐，则返回值的x、y值体现了左上角对齐需要的偏移量。其x、y允许为负（说明格子已超出了形状的范围）。
	 * 3, 前提：形状不能为空。 */
	QPoint updSz();
	
	/** 对齐更新：
	 * 1, 让所有格子在形状内靠左上对齐，不改变所有格子的绝对坐标。
	 * 2, @param offset：所有格子离左上角的偏移量。
	 * 3, 消行时的对齐更新与旋转时的对齐更新会不一样。 */
	Shape & updAlign(const QPoint & offset);
	
	/** 移动：
	 * 1, @param dx：横向移动的距离；为正表示向右移，为负表示向左移。
	 * 2, @param dy：纵向移动的距离；为正表示向下移，为负表示向上移。 */
	inline Shape & move(const int dx, const int dy = 0) {
		m_x += dx;
		m_y += dy;
		return * this;
	}
	
	/** 移动到：
	 * @param x, y：移动到指定的坐标。 */
	inline Shape & move2(const int x, const int y) {
		m_x = x;
		m_y = y;
		return * this;
	}
	
	/** 顺时针方向旋转：
	 * 1, 规则：
	 * . 1.1, 下边缘在游戏面板中的高度保持不变。
	 * . 1.2, 水平位置居中。 */
	Shape & rotateClock();
	
	/** 逆时针方向旋转：
	 * 规则同上。 */
	Shape & rotateAnti();
	
	/**
	 * ================ 游戏存档解析 ================
	 * 详见游戏存档字符串的规则说明。
	 */
	
	/** 解析游戏存档的字符串转换成形状：
	 * 1, @param load ：游戏存档的字符串。
	 * 2, @param cur ：结果参数，字符串流指针的位置，即指向表示形状的字符串的首字符，不能为nullptr。
	 * 3, @param parent ：转换的形状所属的面板。 */
	static Shape * anlys(const QString & load, int * cur, RelatObject * parent);
	
	/** 反解析形状生成游戏存档的字符串：
	 * 1, 格式如："shpSz:4,0:0,0:1,1:1,1:0,"
	 * 2, @param shpNm ：形状名，如"faller"（下落形状）、"next"（下一形状）等，非特殊的形状则名为"shp"。 */
	QString toSaveStr(const QString & shpNm = "shp") const;








	
	/**
	 * ================ 异常 ================
	 * 
	 * 1, 将抛异常封装成函数，可统一异常描述信息，也便于统计本类一共有几种异常、每种抛异常的代码在什么地方。
	 * 2, @param ttl ：异常标题，即抛出异常代码所在的源码文件、函数、行号。调用该函数时传参统一用宏“ET”（详见Exception.h）即可。
	 */
	
	/** 成员格子的指针为NULL指针 */
	inline void nullMbrLtcEx(const QString & ttl) const {
		throw ExTemp<NullPtrEx>(ttl, this, "member lattice is null");
	}
	
	/** 解析异常：
	 * 从数据库中解析游戏存档，生成形状时，出现的异常。 */
	inline static void anlysEx(const QString & ttl, int sz) {
		throw Exception(
					ttl, nullptr, "analyse shape from game loading of database, the shape size is " + QString::number(sz));
	}
	
	/**
	 * ================ Getter/Setter ================
	 */
	
	inline int width() const { return m_w; }
	
	inline Shape & setWidth(const int w) {
		m_w = w;
		return * this;
	}
	
	inline int heigth() const { return m_h; }
	
	inline Shape & setHeight(const int h) {
		m_h = h;
		return * this;
	}
	
	inline int cap() const { return m_cap; }
	
	inline Shape & setCap(const int cap) {
		if(cap > 0) {
			m_cap = cap;
		}
		return * this;
	}
	
	inline const QLinkedList<Lattice *> ls() const { return m_ls; }
	
	inline       QLinkedList<Lattice *> ls()       { return m_ls; }
	
	/**
	 * ================ 内部成员 ================
	 */
protected:
	
	/**
	 * ~~~~~~~~~~~~ 成员变量 ~~~~~~~~~~~~
	 */
	
	int m_w; // 宽
	int m_h; // 高，以格子个数为单位
	int m_cap; // 容量，即最多有几个格子
	QLinkedList<Lattice *> m_ls; // lattice set，格子的集合
	
	/**
	 * ~~~~~~~~~~~~ 内部构造 ~~~~~~~~~~~~
	 */
	
	explicit Shape(RelatObject * parent, int cap);
	
	/**
	 * ~~~~~~~~~~~~ 设置随机位置 ~~~~~~~~~~~~
	 */
	
	Lattice * randLtcClose2Exist(Lattice * ltc);
	
	static Lattice * randLtc(Lattice * ltc, const int w, const int h);
	
	/**
	 * ~~~~~~~~~~~~ 其他 ~~~~~~~~~~~~
	 */

	bool isRowFull(const Lattice * ltc, const int gpWidth);
	
	/**
	 * ================ 调试 ================
	 */
#ifndef DEBUG

public:
	
	void print(const int level = 0) const;
	
#endif // DEBUG
	
};

#endif // SHAPE_H








