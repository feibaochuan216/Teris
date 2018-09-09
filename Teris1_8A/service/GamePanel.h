#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include "RelatObject.h"
#include "Shape.h"
#include "NextPanel.h"
#include "../lib/Mask.h"
#include "../exception/ExTemp.h"
#include <QLinkedList> // 形状的集合
#include <QVector> // 要消行的行号的列表
#include <QTimer>
#include <QPair> // 用于canRoate()判断旋转后需要左右移动多少
#include <cmath> // 用于本局速度计算，得分算法等

extern int OBS_MAX; // 根据关数设置障碍物的初始形状
class NextPanel;
class GameDlg; // 前置声明

/** 游戏面板的业务逻辑：
 * 1, 专门存放、管理下落形状、障碍物，下落形状在它的范围内下落、旋转。
 * 2, 以格子个数为坐标单位。
  
  未完成：

消行时不允许delete faller，空形状就空形状，只有nextFall()时才可以delete，让faller一直保持不为nullptr  

pass()时连续调用两次nextFall()，并delete掉多余的faller
  
 */
class GamePanel : public RelatObject {
public:
	
	/**
	 * ================ 公有成员变量 ================
	 */
	
	Mask state; // 游戏状态标记数组
	enum StateIndx { // 状态数组的每个下标所代表的含义
		IsFallingFast = 0, // 定时器是否以最快速度在下落
		// 这里不能通过m_fallTmr.interval() == FASTEST_SPEED来判断，因为游戏关数到一定级别时，普通速度就已经是最快速度了
		IsPause = 1, // 是否暂停
		IsPressingDown = 2, // Down键是否是按下状态，此时下落形状以最快速度下降
		IsGameOver = 3, // 下落形状已无法再下落，且无法进入游戏面板
		StateSz = 4 // 状态数组的长度
	};
	// 其他判断：
	// 下落计时器是否正在计时用m_fallTmr.isActive()
	
	/**
	 * ================ 构造、析构 ================
	 */
	
	GamePanel(const int level = 1);
	
	/** 禁用拷贝构造 */
private:
	GamePanel(const GamePanel & that);
public:
	
	~GamePanel();
	
	/**
	 * ================ 拷贝赋值 ================
	 * 禁用。
	 */
private:
	GamePanel & operator =(const GamePanel & that);
public:
	
	/**
	 * ================ 判断 ================
	 */
	
	/** 格子是否超出了本游戏面板的范围：
	 * @param ltc：必须是属于本游戏面板的形状的格子。 */
	bool isOut(const Lattice & ltc) const;
	
	/** 下落形状是否可以向指定方向移动：
	 * 1, @param dx：横向移动的距离；为正表示向右移，为负表示向左移。
	 * 2, @param dy：纵向移动的距离；为正表示向下移，为负表示向上移。
	 * 3, @return 能移动则返回true，否则返回false。
	 * 4, 如果下落形状不能下降，且下落形状在游戏面板的上边沿之外，则将IsGameOver的状态设置为true。 */
	bool canMove(const int dx, const int dy = 0);
	
	/** 是否能旋转：
	 * 1, 往哪个方向旋转取决于游戏匹配。
	 * 2, 旋转时可能需要向左右小幅平移cap / 2格以内的距离，以避免和障碍物重叠或超出游戏面板边界。
	 * 3, @param offset：结果参数，旋转后需要水平偏移多少：
	 * . 3.1, > 0 表示需要向右移；
	 * . 3.2, < 0 表示需要向左移；
	 * . 3.3, 为零表示不用偏移。 */
	bool canRotate(int * offset) const;
	
	/** 是否已过关：
	 * 未来的过关条件可能有变
	 */
	inline bool isPass() const;
	
	/**
	 * ================ 查 ================
	 */
	
	/** 统计满行：
	 * @return 满行的行号列表。 */
	QVector<int> fullRows();
	
	/**
	 * ~~~~~~~~~~~~ 像素坐标 ~~~~~~~~~~~~
	 */
	
	virtual int xPix() const;
	
	virtual int yPix() const;
	
	virtual int widthPix() const;
	
	virtual int heightPix() const;
	
	/**
	 * ================ 初始化游戏 ================
	 */
	
	/**
	 * ~~~~~~~~~~~~ 增 ~~~~~~~~~~~~
	 */
	
	/** 追加障碍形状：
	 * 1, 通过指定的高度在游戏面板的底部添加障碍物形状。可用于：
	 * . 1.1, 给障碍物添加初始形状。
	 * . 1.2, 为增加游戏难度将障碍物加高等。
	 * 2, @param obsHeight：必须 > 0。 */
	inline GamePanel & appendObs(const int obsHeight) {
		Shape * shp = new Shape(m_w, obsHeight, this); // m_x默认为零
		shp->set_y(m_h - shp->heigth()); // 初始化形状的位置
		m_obs += shp;
		return * this;
	}
	
	/**
	 * ~~~~~~~~~~~~ 改 ~~~~~~~~~~~~
	 */
	
	/** 初始化新的下落形状的位置：
	 * 前提：m_faller必须非nullptr。 */
	inline GamePanel & initFallerPos() {
		m_faller->set_x((m_w - m_faller->width()) / 2);
		m_faller->set_y(-m_faller->heigth());
		return * this;
	}
	
	/** 移动障碍物：
	 * 1, @param dx：横向移动的距离；为正表示向右移，为负表示向左移。
	 * 2, @param dy：纵向移动的距离；为正表示向下移，为负表示向上移。 */
	inline GamePanel & moveObs(const int dx, const int dy) {
		foreach(Shape * shp, m_obs) {
			if(nullptr == shp) nullShpInObsEx(ET);
			shp->move(dx, dy);
		}
		return * this;
	}
	
	/**
	 * ================ 用户操作处理 ================
	 */
	
	/**
	 * ~~~~~~~~~~~~ 键盘事件处理 ~~~~~~~~~~~~
	 */
	
	void leftKeyPressEvent(); // 代表向左的键被点下时
	
	void rightKeyPressEvent(); // 代表向右的键被点下时
	
	void rotateKeyPressEvent(); // 代表旋转的键被点下时
	
	void downKeyPressEvent(); // 代表向下的键被点下时
	
	void downKeyReleaseEvent(); // 代表向下的键弹起时
	
	/**
	 * ~~~~~~~~~~~~ 改 ~~~~~~~~~~~~
	 */
	
	/** 开始游戏：
	 * 可以是从头开始，也可以是从暂停状态恢复到开始状态。 */
	void start();
	
	/** 暂停游戏：
	 * 下落形状停止下落，其他不变。 */
	void pause();
	
	/**
	 * ================ 情景处理 ================
	 */
	
	/**
	 * ~~~~~~~~~~~~ 定时事件处理 ~~~~~~~~~~~~
	 */
	
	/** 下落计时器到时时：
	 * 由前端的GameDlg负责信号和槽的连接，前端槽函数调用本函数。 */
	void onFallTmOut();
	
	GamePanel & pass(); // 过关
	
	void gameOver(); // 游戏结束
	
	/**
	 * ~~~~~~~~~~~~ 增 ~~~~~~~~~~~~
	 */
	
	/** 加分：
	 * 1, 本局得分和总得分都会增加。
	 * @param rows：要消行的行号集合，不能为空。 */
	inline GamePanel & addScore(const QVector<int> & rows) {
		if(rows.isEmpty()) return * this;
		qulonglong score = pow(2, rows.size() - 1);
		m_score += score;
		m_totalScore += score;
		return * this;
	}
	
	/**
	 * ~~~~~~~~~~~~ 删 ~~~~~~~~~~~~
	 */
	
	/** 消行：
	 * 1, 将格子满了的行删除。
	 * 2, @param rows：要消行的行号的列表，不能为空。
	 * 3, 本函数没有动画效果，一次性将所有要消的格子瞬间全部消除。
	 * 4, 障碍物不能为空。
	 * 5, 自动将被删空的形状从障碍物中去掉。
	 * 6, 所消行以上的格子需要统一降下一行，详见压紧。
	 * 7, @return 可用于集联调用压紧。 */
	GamePanel & rmRows(const QVector<int> & rows);
	
	/** 消除格子：
	 * 1, 用于实现有动画效果的消行，格子可能需要一列一列地删，甚至一个一个地删。
	 * 2, @param row：删除第几行的格子；必须 ≥ 0 且 < m_h。
	 * 3, @param col：删第几列的格子；必须 ≥ 0 且 < m_w。 */
	void rm(const int row, const int col);
	
	/** 压紧：
	 * 1, 把障碍物里被消行以上的格子统一下降，下降几行与消了几行及被消行的位置有关。
	 * 2, @param rows：要消的行的行号列表，不能为空。
	 * 3, 本函数没有动画效果，一次性将所有格子瞬间压紧。
	 * 4, 前提：障碍物不能为空。 */
	GamePanel & compact(const QVector<int> & rows);
	
	/**
	 * ~~~~~~~~~~~~ 改 ~~~~~~~~~~~~
	 */
	
	/** 换新的下落形状：
	 * 1, 前提：下一形状面板的指针m_np不能为nullptr。
	 * 2, 原下落形状自动并入到障碍物。 */
	GamePanel & nextFaller();
	
	/**
	 * ================ 异常 ================
	 */
	
	/** 1, 将抛异常封装成函数，可统一异常描述信息，也便于统计本类一共有几种异常、每种抛异常的代码在什么地方。
	 * 2, @param ttl ：异常标题，即抛出异常代码所在的源码文件、函数、行号。调用该函数时传参统一用宏“ET”（详见Exception.h）即可。 */
	
	/** 障碍物里有NULL的形状指针 */
	inline void nullShpInObsEx(const QString & ttl) const {
		throw ExTemp<NullPtrEx>(ttl, this, "shape in obstracts is null");
	}
	
	/** 障碍物里有NULL的格子指针 */
	inline void nullLtcInObsEx(const QString & ttl, const Shape & shp) const {
		QString msg("member lattice of shape(0x");
		msg += QString::number((long)& shp, 16);
		msg += ") in obs is null";
		throw ExTemp<NullPtrEx>(ttl, this, msg);
	}
	
	/** 空下落形状的指针为NULL */
	inline void nullFallerEx(const QString & ttl) const {
		throw ExTemp<NullPtrEx>(ttl, this, "faller is null");
	}
	
	/** 下落形状的格子指针为NULL */
	inline void nullLtcInFallerEx(const QString & ttl) const {
		throw ExTemp<NullPtrEx>(ttl, this, "lattice of faller is null");
	}
	
	/** 下落形状容量异常：
	 * “下落形状里最多能放几个格子”的设定非法，如为零或-1等。*/
	inline void fallerCapEx(const QString & ttl) const {
		throw Exception(ttl, this, "the capacity of faller < 1");
	}
	
	/** 游戏关级异常，如被设置成第零关、第-1关等 */
	inline void gameLevelEx(const QString & ttl) const {
		throw Exception(ttl, this, "game level < 1");
	}
	
	/** 下落速度异常 */
	inline void speedEx(const QString & ttl) const {
		throw Exception(ttl, this, "speed < 0");
	}
	
	/** 有未关闭的定时器 */
	inline void notClosedTmrEx(const QString & ttl) const {
		throw Exception(ttl, this, "there's a timer without closed");
	}
	
	// QTimerEvent改为QTimer后可能不需要
	
	/** 下落定时器不是被暂停按钮关闭的 */
	inline void fallTmrEx(const QString & ttl) const {
		throw Exception(ttl, this, "fall timer wasn't closed by pause");
	}
	
	/** 定时器状态异常 */
	inline void tmrStateEx(const QString & ttl) const {
		throw Exception(ttl, this, "state TMR is false but falling timer is running");
	}
	
	// QTimerEvent改为QTimer后可能不需要
	
	/** 未知定时器异常 */
	inline void unknownTmrEx(const QString & ttl) const {
		throw Exception(ttl, this, "unknown timer");
	}
	
	// QTimerEvent改为QTimer后可能不需要
	
	inline void nullNpEx(const QString & ttl) const {
		throw ExTemp<NullPtrEx>(ttl, this, "NextPanel pointer in GamePanel is null");
	}
	
	/**
	 * ================ Getter/Setter ================
	 */
	
	inline int level() const { return m_level; }
	
	inline int width() const { return m_w; }
	
	inline GamePanel & setWidth(const int w) { 
		m_w = w;
		return * this;
	}
	
	inline int height() const { return m_h; }
	
	inline GamePanel & setHeight(const int h) {
		m_h = h;
		return * this;
	}
	
	inline const Shape & faller() const { return * m_faller; }
	
	inline       Shape & faller()       { return * m_faller; }
	
	inline const QLinkedList<Shape *> obs() const { return m_obs; }
	
	inline       QLinkedList<Shape *> obs()       { return m_obs; }
	
	inline int speed() const { return m_speed; }
	
	inline qulonglong score() const { return m_score; }
	
	inline qulonglong totalScore() const { return m_totalScore; }
	
	inline qulonglong passScore() const { return m_passScore; }
	
	inline const QTimer & fallTmr() const { return m_fallTmr; }
	
	inline       QTimer & fallTmr()       { return m_fallTmr; }
	
	inline GamePanel & setNp(NextPanel & np) {
		m_np = & np;
		return * this;
	}
	
	inline GamePanel & setClient(GameDlg & gd) {
		m_client = & gd;
		return * this;
	}
	
	/**
	 * ================ 内部成员 ================
	 */
protected:
	
	/**
	 * ~~~~~~~~~~~~ 成员变量 ~~~~~~~~~~~~
	 */
	
	/**
	 * -------- 本局参数 --------
	 */
	
	int m_level; // 关数，第几关，从1开始计
	int m_w; // 宽，即横向有几个格子
	int m_h; // 高，即纵向有几个格子
	Shape * m_faller; // 下落形状
	QLinkedList<Shape *> m_obs; // 障碍物
	int m_speed; // 本局下降速度
	qulonglong m_score; // 本局得分
	qulonglong m_passScore; // 本局通关分数
	qulonglong m_totalScore; // 总得分
	
	/**
	 * -------- 定时器 --------
	 */
	
	QTimer m_fallTmr; // 下落定时器
	
	/**
	 * -------- 关系 --------
	 */
	
	NextPanel * m_np; // 业务逻辑模块中本游戏面板对应的下一形状面板
	GameDlg * m_client; // input and output shower，显示模块中负责输入输出的对象
	
	/**
	 * ~~~~~~~~~~~~ 判断 ~~~~~~~~~~~~
	 */
	
	bool isOut(const Shape & shp) const;
	
	inline bool isLeftOut(const Shape & shp) const {
		if(shp.x() < 0) return true;
		return false;
	}
	
	inline bool isRightOut(const Shape & shp) const {
		if(shp.x() > (m_w - shp.width())) return true;
		return false;
	}
	
	bool isOverlap(const Shape & tmp) const;
	
	/**
	 * ~~~~~~~~~~~~ 初始化游戏 ~~~~~~~~~~~~
	 */
	
	GamePanel & init(int level);
	
	/** 根据关数设置障碍物的初始形状：
	 * 1, @param level：第几关，必须 ≥ 1 。
	 * 2, 算法：关数 / 每过几关增长1次障碍物 * 每次障碍物增长的高度
	 * 3, 会清空原障碍物。 */
	inline GamePanel & setObs(const int level) {
		m_obs.clear();
		int oh = level / OBS_RATE * OBS_INCREM; // obstracts height
		appendObs(oh < OBS_MAX ? oh : OBS_MAX);
		return * this;
	}
	
	/** 下降速度：
	 * 1, 计算本局下落形状的下降速度。
	 * 2, @param level：第几关，必须 ≥ 1 。 */
	inline GamePanel & setSpeed(const int level) {
		m_speed = (int)(SLOWEST_SPEED * pow(SPEED_RATE, level - 1));
		if(m_speed < FASTEST_SPEED) m_speed = FASTEST_SPEED;
		return * this;
	}
	
	/** 过关分数：
	 * 1, 本局得分需要达到多少才能过关。
	 * @param level：第几关，必须 ≥ 1。 */
	inline GamePanel & setPassScore(const int level) {
		m_passScore = (qulonglong)(level * PASS_RATE * PASS_SCORE);
		return * this;
	}
	
	/**
	 * ================ 调试 ================
	 */
#ifndef DEBUG
	
public:
	
	void print(const int level = 0) const;
	
	void printStates(const int level = 0) const;
	
#endif // DEBUG
	
};

#endif // GAMEPANEL_H




