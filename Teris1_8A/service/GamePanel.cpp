#include "GamePanel.h"
#include "../exception/NullPtrException.h"
#include "../exception/OutRangeGeomException.h"
#include <QMessageBox>

extern int GP_WIDTH;
extern int GP_HEIGHT; // 游戏面板默认尺寸
extern int ROTAT_DIR; // 尝试旋转时用到

/**
 * ================ 公有成员变量 ================
 */

const QString GamePanel::m_obsShpExMsg("shape of obstracts is null");
const QString GamePanel::m_obsLtcExMsg("lattice of obstracts is null");
const QString GamePanel::m_fallerExMsg("m_faller is null");

/**
 * ================ 构造、析构 ================
 */

GamePanel::GamePanel(const int level/* = 1*/)
	: Service(nullptr), m_level(level),
	m_w(GP_WIDTH), m_h(GP_HEIGHT), m_faller(new Shape(this)), m_obs(),
	m_speed(SLOWEST), m_score(0), m_passScore(0), m_totalScore(0),
	m_state(6), // 一共有6种状态标记
	m_fallTmr(0),
	m_client(nullptr) { // 必须为NULL，防止循环构造
	if(level < 1) {
		throw Exception(EX_TTL, this, "game level < 1");
	}
	initFallerPos();
	setObs(level);
	setSpeed(level);
	setPassScore(level);
	m_score = 0;
	m_state.fill(false); // 状态标记全部为否
	m_state.setVal(PAUSE, true); // 默认是暂停状态
}

GamePanel::~GamePanel() {
	if(nullptr != m_faller) delete m_faller;
	if(! m_obs.isEmpty()) {
		QMutableLinkedListIterator<Shape *> it(m_obs);
		while(it.hasNext()) {
			Shape * shp = it.next();
			if(nullptr != shp) delete shp;
			it.remove();
		}
	} // m_obs非空
}

/**
 * ================ 判断 ================
 */

bool GamePanel::isOut(const Lattice & ltc) const {
	if(nullptr == ltc.parent()) {
		throw NullParentException(EX_TTL, this);
	} else if(nullptr == ltc.parent()->parent()) {
		throw NullParentException(EX_TTL, this);
	}
	if(ltc.parent()->parent() != this) return false; // 格子不属于本游戏面板
	int x = ltc.xig();
	int y = ltc.yig();
	if(0 <= x && x <= m_w && 0 <= y && y <= m_h) return false;
	return true;
}

bool GamePanel::canMove(const int dx, const int dy/* = 0*/) {
	if(nullptr == m_faller) {
		throw NullPtrException(EX_TTL, this, m_fallerExMsg);
	}
	bool result = true;
	Shape tmp(* m_faller);
	tmp.move(dx, dy);
	if(isOut(tmp) || isOverlap(tmp)) { // 是否越界或重叠
		result = false;
	} // 是否越界或重叠
	if(! result && m_faller->y() + m_faller->heigth() <= 0) {
		m_state.setVal(GAME_OVER, true);
	}
	return result;
}

bool GamePanel::canRotate(int * offset) const {
	if(nullptr == m_faller) {
		throw NullPtrException(EX_TTL, this, m_fallerExMsg);
	}
	if(m_faller->cap() < 1) {
		throw Exception(EX_TTL, this, "cap of m_faller < 1");
	}
	Shape tmp(* m_faller);
	if(RD_CLCK == ROTAT_DIR) tmp.rotateClock(); // 顺时针
	else tmp.rotateAnti(); // 逆时针
	/* left：计数，如果要让形状旋转，它最少要向左平移几格。
	 * right：计数，如果要让形状旋转，它最少要向右平移几格。
	 * max：向左、右各最多能尝试几次。 */
	int left = 0, right = 0;
	const int max = m_faller->cap() / 2;
	if(isLeftOut(tmp)) { // 超出了面板的左边框
		left = max; // 以后不能再向左尝试
		do {
			if(right < max) { // 还能向右尝试
				++right;
				tmp.set_x(m_faller->x() + right);
			} else return false; // 向右的尝试已用尽
		} while(isLeftOut(tmp)); // 超出了面板的左边框
	} // 超出了面板的左边框
	if(isRightOut(tmp)) { // 超出了面板的右边框
		right = max; // 以后不能再向右尝试
		do {
			if(left < max) { // 还能向左尝试
				++left;
				tmp.set_x(m_faller->x() - left);
			} else return false; // 向左的尝试已用尽
		} while(isRightOut(tmp)); // 超出了面板的右边框
	} // 超出了面板的右边框
	while(isOverlap(tmp) && right < max) { // 与障碍物重叠，且还能向右尝试
		++right;
		tmp.set_x(m_faller->x() + right);
	}
	while(isOverlap(tmp) && left < max) { // 与障碍物重叠，且还能向左尝试
		++left;
		tmp.set_x(m_faller->x() - left);
	}
	if(left >= max && right >= max) return false; // 两个方向的尝试都已用尽
	if(nullptr != offset) * offset = left < right ? -left : right; // 左右距离取小者
	return true;
} // canRotate()

/** 未来的过关条件可能有变 */
bool GamePanel::isPass() const {
	if(m_score >= m_passScore) return true;
	return false;
}

/**
 * ================ 查 ================
 */

QVector<int> GamePanel::fullRows() {
	QVector<int> v; // 用于统计每行格子数量；下标为行号
	for(int i = 0; i < m_h; ++i) v += 0;
	/* 统计下落形状 */
	if(nullptr == m_faller) {
		throw NullPtrException(EX_TTL, this, m_fallerExMsg);
	}
	foreach(const Lattice * ltc, m_faller->ls()) {
		if(nullptr == ltc) {
			throw NullPtrException(EX_TTL, this, "lattice of faller is null");
		}
		++v[ltc->yig()];
	}
	/* 统计障碍物 */
	foreach(const Shape * shp, m_obs) {
		if(nullptr == shp) {
			throw NullPtrException(EX_TTL, this, m_obsShpExMsg);
		}
		foreach(const Lattice * ltc, shp->ls()) {
			if(nullptr == ltc) {
				throw NullPtrException(
							EX_TTL, this, m_obsLtcExMsg);
			}
			++v[ltc->yig()];
		}
	}
	QVector<int> rows;
	for(int i = 0; i < m_h; ++i) {
		if(v[i] == m_w) rows += i;
	}
	return rows;
} // fullRows()

/**
 * ~~~~~~~~~~~~ 像素坐标 ~~~~~~~~~~~~
 */

int GamePanel::xPix() const {
	return m_client->xPixGp();
}

int GamePanel::yPix() const {
	return m_client->yPixGp();
}

int GamePanel::widthPix() const {
	return m_w * LTC_SZ;
}

int GamePanel::heightPix() const {
	return m_h * LTC_SZ;
}

/**
 * ================ 用户操作处理 ================
 */

/**
 * ~~~~~~~~~~~~ 键盘事件处理 ~~~~~~~~~~~~
 * 玩家具体按的是哪个键由前端去判断，业务逻辑类不用去管这些。因为前端可以设置多个键有相同的功能。
 * 如：控制下落形状不只上、下、左、右键，还可以是A、S、W、D键，等。
 * 如：控制下落形状旋转可以不只是上键，还可以是回车或空格键，等。
 */

void GamePanel::leftKeyPressEvent() {
	if(! m_state[PAUSE] && canMove(-1, 0)) {
		m_faller->move(-1, 0);
		m_client->update();
	}
}

void GamePanel::rightKeyPressEvent() {
	if(! m_state[PAUSE] && canMove(1, 0)) {
		m_faller->move(1, 0);
		m_client->update();
	}
}

void GamePanel::rotateKeyPressEvent() {
	if(! m_state[PAUSE]) {
		int offset = 0;
		if(canRotate(& offset)) {
			if(RD_CLCK == ROTAT_DIR) m_faller->rotateClock();
			else m_faller->rotateAnti();
			m_faller->move(offset);
			m_client->update();
		} // can rotate
	} // ! pause
}

void GamePanel::downKeyPressEvent() {
	if(! m_state[PAUSE]) { // 非暂停状态下
		m_state.setVal(PRESSING, true);
		if(! m_state[PRESSED]) { // 之前没有按过Down键
			m_state.setVal(PRESSED, true);
			// 标记，本轮下落已按过Down键，无论玩家再按什么，下落形状的速度都不能回到普通速度
			if(! m_state[PAUSE] && ! m_state[TMR]) { // 未暂定但没有定时器在跑
				m_fallTmr = m_client->startTimer(FASTEST);
				m_state.setVal(TMR, true);
				m_state.setVal(FAST, true);
			} else if(! m_state[PAUSE] && ! m_state[FAST]) { // 未暂停且有定时器在跑，但是是普通速度的
				m_client->killTimer(m_fallTmr);
				m_fallTmr = m_client->startTimer(FASTEST);
				m_state.setVal(FAST, true);
			} // 有定时器跑，但是是普通速度的
		} // 之前没有按过Down键
	} // 非暂停状态下
} // downKeyPressEvent()

void GamePanel::downKeyReleaseEvent() {
	m_state.setVal(PRESSING, false);
}

/**
 * ~~~~~~~~~~~~ 改 ~~~~~~~~~~~~
 */

void GamePanel::start() {
	if(m_speed < 0) {
		throw Exception(EX_TTL, this, "speed < 0");
	}
	if(m_state[TMR]) { // 有定时器在跑
		throw Exception(EX_TTL, this, "there's a timer without stopped");
	}
	m_state.setVal(PAUSE, false);
	if(m_state[PRESSING] || m_state[PRESSED]) {
		m_fallTmr = m_client->startTimer(FASTEST);
		m_state.setVal(FAST, true);
	} else {
		m_fallTmr = m_client->startTimer(m_speed);
		m_state.setVal(FAST, false);
	}
	m_state.setVal(TMR, true); // 标记有定时器在跑
}

void GamePanel::pause() {
	if(! m_state[TMR]) { // 没有定时器在跑
		throw Exception(EX_TTL, this, "fall timer wasn't stopped by pause");
	}
	m_state.setVal(PAUSE, true);
	m_client->killTimer(m_fallTmr);
	m_state.setVal(TMR, false);
	m_state.setVal(FAST, false);
}

void GamePanel::stop() {
	m_state.setVal(PAUSE, false);
	m_state.setVal(PRESSED, false);
	m_state.setVal(GAME_OVER, true);
	if(m_state[TMR]) { // 有定时器在跑
		m_client->killTimer(m_fallTmr);
		m_state.setVal(TMR, false);
	}
	m_state.setVal(FAST, false);
}

/**
 * ================ 情景处理 ================
 */

/**
 * ~~~~~~~~~~~~ 定时事件处理 ~~~~~~~~~~~~
 */

void GamePanel::timerEvent(QTimerEvent * ev) {
	if(nullptr == ev) {
		throw NullPtrException(EX_TTL, this, "QTimerEvent pointer is null");
	}
	if(! m_state[TMR]) { // 状态标记却显示没有定时器在跑
		throw Exception(
					EX_TTL, this,
					"state TMR is false but there's a running timer: "
					+ QString::number(m_fallTmr));
	}
	if(ev->timerId() == m_fallTmr) { // 下落定时器到时
		if(m_state[PAUSE]) { // 当前为暂停状态
			m_client->killTimer(m_fallTmr);
			m_state.setVal(TMR, false);
			m_state.setVal(FAST, false);
		} else { // 当前非暂停状态
			if(canMove(0, 1)) { // 下落形状能下降
				m_faller->move(0, 1);
				m_client->update(); // 刷新显示
				return;
			} else { // 下落形状不能下降
				m_state.setVal(PRESSED, false); // 本轮下降已结束，本轮标记恢复默认
				m_client->killTimer(m_fallTmr);
				m_state.setVal(TMR, false);
				m_state.setVal(FAST, false); // 先关闭定时器
				if(m_state[GAME_OVER]) { // 游戏结束
					gameOver();
					return;
				} // 游戏结束
				if(isPass()) { // 过关
					pass();
					return;
				} // 过关
				/* 
				 * 下落形状不能下降，且既没有过关，游戏也没有结束
				 */
				QVector<int> rows = fullRows();
				if(! rows.isEmpty()) { // 需要消行
					rmRows(rows); // 消行
					// m_client->rmRows(rows); // 有消行动画效果时
					compact(rows); // 压紧
					// m_client->compact(rows); // 有消行动画效果时
					addScore(rows); // 加分
					// m_client->addScore(rows); // 加分动画效果
					m_client->update(); // 刷新显示
				} // 需要消行
				nextFaller(); // 更新下落形状
				initFallerPos(); // 初始化下落形状的位置
				/*
				 * 恢复下降定时器
				 */
				if(m_state[PRESSING]) { // 此时Down键是按下状态
					m_fallTmr = m_client->startTimer(FASTEST);
					m_state.setVal(FAST, true); // 普通定时器改为快速定时器
				} else { // 此时Down键是抬起状态
					m_fallTmr = m_client->startTimer(m_speed);
					m_state.setVal(FAST, false); // 快速定时器改为普通定时器
				}
				m_state.setVal(TMR, true);
			} // 下落形状不能下降
		} // 当前非暂停状态
	} else { // 定时器不是下落定时器
		throw Exception(
					EX_TTL, this, "unknown timer id: "
					+ QString::number(m_fallTmr)); // 定时器非法
	} // 定时器不是下落定时器
} // timerEvent()

/**
 * ~~~~~~~~~~~~ 过关 ~~~~~~~~~~~~
 */

GamePanel & GamePanel::pass() {
	++m_level;
	if(nullptr != m_faller) {
		delete m_faller;
	}
	if(nullptr == m_np) {
		throw NullPtrException(EX_TTL, this, "GamePanel's m_np is null");
	}
	m_np->refresh();
	if(nullptr != m_faller) delete m_faller;
	m_faller = new Shape(this);
	initFallerPos();
	if(! m_obs.isEmpty()) {
		QMutableLinkedListIterator<Shape *> it(m_obs);
		while(it.hasNext()) {
			Shape * shp = it.next();
			if(nullptr != shp) delete shp;
			it.remove();
		}
	}
	setObs(m_level);
	setSpeed(m_level);
	m_score = 0;
	setPassScore(m_level);
	if(0 != m_fallTmr) {
		m_client->killTimer(m_fallTmr);
		m_fallTmr = 0;
	}
	m_state.fill(false); // 状态标记全部为否
	return * this;
} // pass()

/**
 * ~~~~~~~~~~~~ 游戏结束 ~~~~~~~~~~~~
 */

void GamePanel::gameOver() {
	if(m_state[TMR]) {
		m_client->killTimer(m_fallTmr);
		m_state.setVal(TMR, false);
		m_state.setVal(FAST, false);
	}
	m_state.setVal(PAUSE, false);
	m_state.setVal(PRESSED, false);
	m_state.setVal(GAME_OVER, true);
	m_client->gameOver(); // 显示Game Over的动画
}

/**
 * ~~~~~~~~~~~~ 删 ~~~~~~~~~~~~
 */

GamePanel & GamePanel::rmRows(const QVector<int> & rows) {
	if(rows.isEmpty()) return * this;
	if(nullptr == m_faller) {
		throw NullPtrException(EX_TTL, this, m_fallerExMsg);
	}
	if(0 != m_faller->rmRows(rows) && m_faller->ls().isEmpty()) {
		delete m_faller;
		m_faller = nullptr;
	}
	QMutableLinkedListIterator<Shape *> it(m_obs);
	while(it.hasNext()) {
		Shape * shp = it.next();
		if(nullptr == shp) {
			throw NullPtrException(EX_TTL, this, m_obsShpExMsg);
		}
		if(0 != shp->rmRows(rows) && shp->ls().isEmpty()) {
			it.remove();
		}
	}
	return * this;
}

void GamePanel::rm(const int row, const int col) {
	if(row < 0 || row >= m_h || col < 0 || col >= m_w) return;
	if(nullptr == m_faller) {
		throw NullPtrException(EX_TTL, this, m_fallerExMsg);
	}
	if(m_faller->rm(row, col) && m_faller->ls().isEmpty()) {
		// 下落形状被删空
		delete m_faller;
		m_faller = nullptr;
	}
	if(m_obs.isEmpty()) return;
	QMutableLinkedListIterator<Shape *> it(m_obs);
	while(it.hasNext()) {
		Shape * shp = it.next();
		if(nullptr == shp) {
			throw NullPtrException(EX_TTL, this, m_obsShpExMsg);
		}
		if(shp->rm(row, col) && shp->ls().isEmpty()) {
			delete shp;
			shp = nullptr;
			it.remove();
		}
	}
}

GamePanel & GamePanel::compact(const QVector<int> & rows) {
	if(rows.isEmpty()) return * this;
	if(nullptr != m_faller && ! m_faller->ls().isEmpty()) {
		// 下落形状不为空（下落形状有可能在消行时被删空）
		foreach(Lattice * ltc, m_faller->ls()) {
			if(nullptr == ltc) {
				throw NullPtrException(
							EX_TTL, this, "lattice of faller is null");
			}
			for(int i = 0; i < rows.size(); ++i) {
				if(ltc->yig() < rows[i]) ltc->move(0, 1);
			}
		} // 遍历下落形状
	} // 下落形状不为空
	foreach(Shape * shp, m_obs) {
		if(nullptr == shp) {
			throw NullPtrException(EX_TTL, this, m_obsShpExMsg);
		}
		shp->rmRows(rows);
	} // 遍历障碍物
	return * this;
}

/**
 * ~~~~~~~~~~~~ 改 ~~~~~~~~~~~~
 */

GamePanel & GamePanel::nextFaller() {
	if(nullptr == m_np) {
		throw NullPtrException(
					EX_TTL, this, "NextPanel * in gamePanel is null");
	}
	if(nullptr != m_faller) {
		m_obs += m_faller;
	} // else m_faller为NULL可能是因为消行把整个m_faller都给消没了
	m_faller = & m_np->give();
	m_faller->setParent(this);
	return * this;
}

/**
 * ================ Getter/Setter ================
 */

bool GamePanel::isGameOver() const {
	return m_state[GAME_OVER];
}

/**
 * ================ 仅内部使用 ================
 */

/**
 * ~~~~~~~~~~~~ 判断 ~~~~~~~~~~~~
 */

/** 指定形状是否超出了本游戏面板的范围：
 * 1, @param shp：必须是属于本游戏面板内的形状。
 * 2, 形状向上超出游戏面板的距离不超过形状高度时，不算超出。 */
bool GamePanel::isOut(const Shape & shp) const {
	if(shp.y() + shp.heigth() > m_h || shp.x() < 0 
	   || shp.x() + shp.width() > m_w || shp.y() < -shp.heigth()) {
		return true;
	}
	return false;
}

/** 指定的形状是否与障碍物有重叠：
 * 用于判断下落形状是否可以移动、旋转等。
 * @param tmp：必须是属于本游戏面板内的形状。 */
bool GamePanel::isOverlap(const Shape & tmp) const {
	foreach(const Shape * shp, m_obs) {
		if(nullptr == shp) {
			throw NullPtrException(EX_TTL, this, m_obsShpExMsg);
		}
		if(shp->isOverlap(tmp)) return true;
	}
	return false;
}

/**
 * ================ 调试 ================
 */
#ifndef DEBUG

void GamePanel::print(const int level/* = 0*/) const {
	indent(level);
	cout << "GamePanel: " << this << " = {" << "parent: " << m_parent
		 << ", w = " << m_w << ", h = " << m_h << ',' << endl;
	indent(level + 1);
	cout << "xPix = " << xPix() << ", yPix = " << yPix()
		 << ", wPix = " << widthPix() << ", hPix = " << heightPix()
		 << ", client: " << m_client << " = \'"
		 << m_client->objectName().toStdString() << "\'," << endl;
	indent(level + 1);
	cout << "speed = " << m_speed << ", score = " << m_score 
		 << ", totalScore = " << m_totalScore << ", passScore = " << m_passScore
		 << endl;
	indent(level + 1);
	cout << boolalpha << "state = {FALL : " << m_state[PAUSE]
			<< ", PRESSED : " << m_state[PRESSED]
			   << ", PRESSING : " << m_state[PRESSING] 
				  << ", GAME_OVER : " << m_state[GAME_OVER] << "}, faller:";
	indent(level);
	if (nullptr == m_faller) {
		cout << " nullptr" << endl;
	} else {
		cout << endl;
		m_faller->print(level + 1);
	}
	indent(level + 1);
	cout << "obs: " << & m_obs << " = [" << endl;
	foreach(const Shape * shp, m_obs) {
		shp->print(level + 2);
	}
	indent(level);
	cout << "]}" << endl;
}

void GamePanel::printStates(const int level/* = 0*/) const {
	indent(level);
	cout << "GamePanel: " << boolalpha << " states = {PAUSE : " << m_state[PAUSE]
			<< ", PRESSED : " << m_state[PRESSED]
			   << ", PRESSING : " << m_state[PRESSING] 
				  << ", GAME_OVER : " << m_state[GAME_OVER]
					 << "}, speed = " << m_speed << endl;
}


#endif // DEBUG












