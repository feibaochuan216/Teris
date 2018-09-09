#include "GamePanel.h"
#include "../show/GameDlg.h"
#include <QMessageBox>
#include <QMap>

extern int GP_WIDTH;
extern int GP_HEIGHT; // 游戏面板默认尺寸
extern int ROTAT_DIR; // 尝试旋转时用到

/**
 * ================ 构造、析构 ================
 */

GamePanel::GamePanel(const int level/* = 1*/)
	: RelatObject(nullptr), state(StateSz), m_level(level),
	m_w(GP_WIDTH), m_h(GP_HEIGHT), m_faller(new Shape(this)), m_obs(),
	m_speed(SLOWEST_SPEED), m_score(0), m_passScore(0),
	m_totalScore(0), m_client(nullptr) { // 必须为NULL，防止循环构造
	if(level < 1) gameLevelEx(ET);
	init(level); // 初始化游戏面板
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
		ltc.nullParentEx(ET);
	} else if(nullptr == ltc.parent()->parent()) {
		((Shape *)ltc.parent())->nullParentEx(ET);
	}
	if(ltc.parent()->parent() != this) return false; // 格子不属于本游戏面板
	int x = ltc.xig();
	int y = ltc.yig();
	if(0 <= x && x <= m_w && 0 <= y && y <= m_h) return false;
	return true;
}

bool GamePanel::canMove(const int dx, const int dy/* = 0*/) {
	if(nullptr == m_faller) nullFallerEx(ET);
	bool result = true;
	Shape tmp(* m_faller);
	tmp.move(dx, dy);
	if(isOut(tmp) || isOverlap(tmp)) { // 是否越界或重叠
		result = false;
	} // 是否越界或重叠
	if(! result && m_faller->y() + m_faller->heigth() <= 0) {
		state.setVal(IsGameOver, true);
	}
	return result;
}

bool GamePanel::canRotate(int * offset) const {
	if(nullptr == m_faller) nullFallerEx(ET);
	if(m_faller->cap() < 1) fallerCapEx(ET);
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
	QMap<int, int> map; // 行号与该行格子个数的映射
	for(int i = 0; i < m_h; ++i) {
		map.insert(i, 0);
	}
	if(nullptr == m_faller) nullFallerEx(ET);
	foreach(const Lattice * ltc, m_faller->ls()) {
		if(nullptr == ltc) nullLtcInFallerEx(ET);
		map.insert(ltc->y(), map.value(ltc->y()) + 1);
	}
	if(! m_obs.isEmpty()) {
		foreach(const Shape * shp, m_obs) {
			if(nullptr == shp) nullShpInObsEx(ET);
			foreach(const Lattice * ltc, shp->ls()) {
				if(nullptr == ltc) nullLtcInObsEx(ET, * shp);
				map.insert(ltc->y(), map.value(ltc->y()) + 1);
			}
		}
	}
	
	
	
	int i = 0;
	foreach(int cnt, map) {
		cout << cnt << " ";
		++i;
		if(i % m_w == 0) cout << endl;
	}
	cout << endl;
	
	//DEBUG
	
	
	QVector<int> res;
	QMapIterator<int, int> it(map);
	while(it.hasNext()) {
		int row = it.peekNext().key();
		int cnt = it.next().value();
		if(cnt == m_w) res += row;
	}
	return res;
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
	if(! state[IsPause] && canMove(-1, 0)) {
		m_faller->move(-1, 0);
		m_client->update();
	}
}

void GamePanel::rightKeyPressEvent() {
	if(! state[IsPause] && canMove(1, 0)) {
		m_faller->move(1, 0);
		m_client->update();
	}
}

void GamePanel::rotateKeyPressEvent() {
	if(! state[IsPause]) {
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
	state.setVal(IsPressingDown, true);
	/* 1, 如果当前为暂停状态，或者已经在以最快速度正在下落，则什么都不做。
	 * 2, 否则，重新以最快速度为间隔时间开始下落计时。 */
	if(! state[IsPause]) {
		state.setVal(IsFallingFast, true);
		m_fallTmr.setInterval(FASTEST_SPEED);
		if(! m_fallTmr.isActive()) {
			m_fallTmr.start();
		}
	}
}

void GamePanel::downKeyReleaseEvent() {
	state.setVal(IsPressingDown, false);
	state.setVal(IsFallingFast, false);
	if(m_speed < 0) speedEx(ET);
	m_fallTmr.setInterval(m_speed);
}

/**
 * ~~~~~~~~~~~~ 改 ~~~~~~~~~~~~
 */

void GamePanel::start() {
	state.setVal(IsPause, false); // 不再暂停
	m_fallTmr.start();
}

void GamePanel::pause() {
	state.setVal(IsPause, true);
	m_fallTmr.stop();
}

/**
 * ================ 情景处理 ================
 */

/**
 * ~~~~~~~~~~~~ 定时事件处理 ~~~~~~~~~~~~
 */

void GamePanel::onFallTmOut() {
	if(canMove(0, 1)) { // 下落形状能下降
		m_faller->move(0, 1);
		m_client->update(); // 刷新显示
		return;
	} else { // 下落形状不能下降
		m_fallTmr.stop();
		if(state[IsGameOver]) { // canMove()会设置IsGameOver状态
			gameOver();
			return;
		} else if(isPass()) { // 已过关
			pass();
			return;
		}
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
		if(! state[IsPause]) m_fallTmr.start(); // 重新开始下落
	} // 下落形状不能下降
}

/**
 * ~~~~~~~~~~~~ 过关 ~~~~~~~~~~~~
 */

GamePanel & GamePanel::pass() {
	if(nullptr == m_np) nullNpEx(ET);
	nextFaller();
	nextFaller(); // 调用两次以刷新下落形状，以及下一形状面板里的预备形状
	// 原下落形状会自动添加到障碍物里
	/* 释放障碍物里的所有形状 */
	if(! m_obs.isEmpty()) {
		QMutableLinkedListIterator<Shape *> it(m_obs);
		while(it.hasNext()) {
			Shape * shp = it.next();
			if(nullptr != shp) delete shp;
			it.remove();
		}
	}
	++m_level;
	init(m_level); // 初始化游戏面板
	return * this;
} // pass()

/**
 * ~~~~~~~~~~~~ 游戏结束 ~~~~~~~~~~~~
 */

void GamePanel::gameOver() {
	
	m_client->gameOver(); // 显示Game Over的动画
}

/**
 * ~~~~~~~~~~~~ 删 ~~~~~~~~~~~~
 */

GamePanel & GamePanel::rmRows(const QVector<int> & rows) {
	if(rows.isEmpty()) return * this;
	if(nullptr == m_faller) nullFallerEx(ET);
	m_faller->rmRows(rows);
	QMutableLinkedListIterator<Shape *> it(m_obs);
	while(it.hasNext()) {
		Shape * shp = it.next();
		if(nullptr == shp) nullShpInObsEx(ET);
		if(shp->rmRows(rows) > 0 && shp->ls().isEmpty()) {
			it.remove();
			delete shp; // 如果形状被删空了，就从障碍物中删除并释放
		}
	}
	return * this;
}

void GamePanel::rm(const int row, const int col) {
	if(row < 0 || row >= m_h || col < 0 || col >= m_w) return;
	if(nullptr == m_faller) nullFallerEx(ET);
	if(m_faller->rm(row, col) && m_faller->ls().isEmpty()) {
		// 下落形状被删空
		delete m_faller;
		m_faller = nullptr;
	}
	if(m_obs.isEmpty()) return;
	QMutableLinkedListIterator<Shape *> it(m_obs);
	while(it.hasNext()) {
		Shape * shp = it.next();
		if(nullptr == shp) nullShpInObsEx(ET);
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
			if(nullptr == ltc) nullLtcInFallerEx(ET);
			for(int i = 0; i < rows.size(); ++i) {
				if(ltc->yig() < rows[i]) ltc->move(0, 1);
			}
		} // 遍历下落形状
	} // 下落形状不为空
	foreach(Shape * shp, m_obs) {
		if(nullptr == shp) nullShpInObsEx(ET);
		shp->rmRows(rows);
	} // 遍历障碍物
	return * this;
}

/**
 * ~~~~~~~~~~~~ 改 ~~~~~~~~~~~~
 */

GamePanel & GamePanel::nextFaller() {
	if(nullptr == m_np) nullNpEx(ET);
	if(nullptr == m_faller) nullFallerEx(ET);
	if(m_faller->ls().isEmpty()) {
		delete m_faller;
		// m_faller可能会因为消行把所有格子都给消没了
	} else {
		m_obs += m_faller; // 非空的下落形状并入到障碍物中
	}
	m_faller = & m_np->give();
	m_faller->setParent(this);
	initFallerPos(); // 初始化下落形状的位置
	return * this;
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
		if(nullptr == shp) nullShpInObsEx(ET);
		if(shp->isOverlap(tmp)) return true;
	}
	return false;
}

/**
 * ~~~~~~~~~~~~ 初始化游戏 ~~~~~~~~~~~~
 */

/** 初始化游戏面板：
 * @param level ：必须 > 0 。 */
GamePanel & GamePanel::init(int level) {
	m_level = level;
	initFallerPos(); // 初始化下落形状的位置
	setObs(level); // 初始化障碍物
	setSpeed(level); // 初始化下落速度
	m_fallTmr.setInterval(m_speed); // 初始化下落定时器的间隔时间
	setPassScore(level); // 初始化过关需要的分数
	m_score = 0; // 初始化本局分数
	state.fill(false); // 状态标记全部为否
	state.setVal(IsPause, true); // 暂停状态默认为真
	return * this;
}

/**
 * ================ 调试 ================
 */
#ifndef DEBUG

void GamePanel::print(const int level/* = 0*/) const {
	indent(level);
	cout << "GamePanel(" << this << "):{" << "parent: " << m_parent
		 << ", w: " << m_w << ", h: " << m_h << ',' << endl;
	indent(level + 1);
	cout << "xPix: " << xPix() << ", yPix: " << yPix()
		 << ", wPix: " << widthPix() << ", hPix: " << heightPix()
		 << ", client(" << m_client << "): \'"
		 << m_client->objectName().toStdString() << "\'," << endl;
	indent(level + 1);
	cout << "speed: " << m_speed << ", score: " << m_score 
		 << ", totalScore: " << m_totalScore << ", passScore: " << m_passScore
		 << endl;
	indent(level + 1);
	cout << boolalpha << "state: {FALL: " << state[IsPause]
			<< ", PRESSING: " << state[IsPressingDown] 
			   << ", GAME_OVER: " << state[IsGameOver] << "}, faller:";
	indent(level);
	if (nullptr == m_faller) {
		cout << " nullptr" << endl;
	} else {
		cout << endl;
		m_faller->print(level + 1);
	}
	indent(level + 1);
	cout << "obs(" << & m_obs << "):[" << endl;
	foreach(const Shape * shp, m_obs) {
		shp->print(level + 2);
	}
	indent(level);
	cout << "]}" << endl;
}

void GamePanel::printStates(const int level/* = 0*/) const {
	indent(level);
	cout << "GamePanel: " << boolalpha << " states: {PAUSE: " << state[IsPause]
			<< ", PRESSING: " << state[IsPressingDown] 
			   << ", GAME_OVER: " << state[IsGameOver]
				  << "}, speed: " << m_speed << endl;
}


#endif // DEBUG












