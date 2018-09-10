#include "GameDlg.h"
#include "ui_GameDlg.h"
#include "SetDlg.h"
#include <QPainter>
#include <QPaintEvent>
#include <QMessageBox>

#include <QDebug>

extern int GP_WIDTH;
extern int GP_HEIGHT;
extern int LTC_SZ;
extern int SHP_SZ;

GameDlg::GameDlg(int level, QWidget * parent/* = nullptr*/)
	: QDialog(parent), ui(new Ui::GameDlg),
	  gp(new GamePanel(level)), np(new NextPanel) {
	ui->setupUi(this);
	
	/* 初始化显示模块与业务逻辑模块之间的传递者 */
	gp->setClient(* this);
	gp->setNp(* np);
	np->setClient(* this);
	
	/* 初始化界面 */
	ui->gamePanel->setMinimumSize(gp->widthPix(), gp->heightPix());
	ui->nextPanel->setMinimumSize(np->widthPix(), np->heightPix());
	// 下一形状面板的上、下、左、右多留一个格子的宽度
	update();
	
	connect(ui->startBtn, SIGNAL(clicked()), this, SLOT(start()));
	connect(ui->pauseBtn, SIGNAL(clicked()), this, SLOT(pause()));
	connect(& gp->fallTmr(), SIGNAL(timeout()), this, SLOT(onFallTmOut()));
	connect(ui->setBtn, SIGNAL(clicked()), this, SLOT(setDlg()));
	connect(ui->exitBtn, SIGNAL(clicked()), this, SLOT(exit()));
}

GameDlg::~GameDlg() {
	delete ui;
}

void GameDlg::paintEvent(QPaintEvent *) {
	ui->level->setText("第 " + QString::number(gp->level()) + " 关");
	QString score(QString::number(gp->score()));
	/* 分数字符很长时，用逗号分隔 */
	if(score.size() > 3) {
		for(int i = score.size() - 3; i > 0; i -= 3) {
			score.insert(i, ',');
		}
	}
	ui->score->setText(score);
	score = QString::number(gp->totalScore());
	/* 分数字符很长时，用逗号分隔 */
	if(score.size() > 3) {
		for(int i = score.size() - 3; i > 0; i -= 3) {
			score.insert(i, ',');
		}
	}
	ui->totalScore->setText(score);
	
	QPainter painter(this);
	painter.setPen(QColor(0, 0, 0));
	painter.setBrush(QColor(0, 0xff, 0));
	
	paintGp(painter);
	paintNp(painter);
}

/**
 * ~~~~~~~~~~~~ 底层实现 ~~~~~~~~~~~~
 */

void GameDlg::paint(const Lattice & ltc, QPainter & painter) {
	painter.drawRect(QRect(ltc.xPix(), ltc.yPix(), LTC_SZ, LTC_SZ));
}

void GameDlg::paint(const Shape & shp, QPainter & painter) {
	foreach(const Lattice * ltc, shp.ls()) {
		paint(* ltc, painter);
	}
}

void GameDlg::paintFaller(QPainter & painter) {
	foreach(const Lattice * ltc, gp->faller().ls()) {
		if(nullptr == ltc) {
			gp->nullLtcInFallerEx(ET);
		}
		if(! gp->isOut(* ltc)) paint(* ltc, painter);
	}
}

void GameDlg::paintGp(QPainter & painter) {
	paintFaller(painter); // 绘制下落形状
	foreach(const Shape * shp, gp->obs()) {
		paint(* shp, painter);
	}
}

void GameDlg::paintNp(QPainter & painter) {
	paint(np->peek(), painter);
}

/**
 * ================ 键盘事件处理 ================
 */

void GameDlg::keyPressEvent(QKeyEvent * ev) {
	const int key = ev->key();
	if(Qt::Key_Left == key || Qt::Key_A == key) {
		gp->leftKeyPressEvent();
	} else if(Qt::Key_Right == key || Qt::Key_D == key) {
		gp->rightKeyPressEvent();
	} else if(Qt::Key_Up == key || Qt::Key_W == key) {
		gp->rotateKeyPressEvent();
	} else if(Qt::Key_Down == key || Qt::Key_S == key) {
		gp->downKeyPressEvent();
	} else if(Qt::Key_Space == key || Qt::Key_Enter == key) {
		if(gp->state[GamePanel::IsPause]) start();
		else pause();
	} else if(Qt::Key_Tab == key) {
		gp->cheat();
	}
}

void GameDlg::keyReleaseEvent(QKeyEvent * ev) {
	const int key = ev->key();
	if(Qt::Key_Down == key || Qt::Key_S == key) {
		gp->downKeyReleaseEvent();
	}
}

/**
 * ================ 槽 ================
 */

void GameDlg::start() {
	ui->startBtn->setEnabled(false);
	ui->pauseBtn->setEnabled(true);
	gp->start();
}

void GameDlg::pause() {
	ui->startBtn->setEnabled(true);
	ui->pauseBtn->setEnabled(false);
	gp->pause();
}

void GameDlg::onFallTmOut() {
	gp->onFallTmOut();
}

void GameDlg::setDlg() {
	pause();
	SetDlg sd(this);
	sd.exec();
	
	
	// 未完成
	
	
	
}

void GameDlg::exit() {
	pause();
	if(QMessageBox::Yes
	   == QMessageBox(
		   QMessageBox::Warning, "俄罗斯方块", "是否退出游戏？",
		   QMessageBox::Yes | QMessageBox::No, this).exec()) {
		close();
	}
}

/**
 * ================ 动画效果 ================
 */

void GameDlg::gameOver() {
	if(QMessageBox::Yes == QMessageBox(
				QMessageBox::Warning, "俄罗斯方块", "游戏结束！是否重来？",
		   QMessageBox::Yes | QMessageBox::No, this).exec()) {
		close();
		// 暂时全是关闭窗口
	} else {
		close();
	}
}

/**
 * ================ Getter/Setter ================
 */

int GameDlg::xPixGp() const {
	return ui->gamePanel->x();
}

int GameDlg::yPixGp() const {
	return ui->gamePanel->y();
}

int GameDlg::xPixNp() const {
	return ui->nextPanel->x();
}

int GameDlg::yPixNp() const {
	return ui->nextPanel->y();
}







