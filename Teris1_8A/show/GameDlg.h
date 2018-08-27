#ifndef GAMEDLG_H
#define GAMEDLG_H

#include "service/GamePanel.h"
#include "service/NextPanel.h"
#include <QDialog>

class GamePanel;
class NextPanel; // 前置声明

namespace Ui {
	class GameDlg;
}

class GameDlg : public QDialog {
	Q_OBJECT
	
public:
	explicit GameDlg(QWidget * parent = nullptr);
	~GameDlg();
	
	/**
	 * ================ 绘制 ================
	 */
	
	void paintEvent(QPaintEvent * ev);
	
	void paint(const Lattice & ltc, QPainter & painter);
	
	void paint(const Shape & shp, QPainter & painter);
	
	void paintFaller(QPainter & painter);
	
	void paintGp(QPainter & painter);
	
	void paintNp(QPainter & painter);
	
	/**
	 * ================ 定时事件处理 ================
	 */
	
	void timerEvent(QTimerEvent * ev);
	
	/**
	 * ================ 键盘事件处理 ================
	 */
	
	void keyPressEvent(QKeyEvent * ev);
	
	void keyReleaseEvent(QKeyEvent * ev);
	
	/**
	 * ================ 槽 ================
	 */
public slots:
	void start();
	
	void pause();
	
	void setDlg();
	
	void exit();
	
	/**
	 * ================ 动画效果 ================
	 */
	
	void gameOver();
	
	/**
	 * ================ Getter/Setter ================
	 */
	
	/** 游戏面板的像素坐标 */
	int xPixGp() const;
	
	int yPixGp() const;
	
	/** 下一形状面板的像素坐标 */
	int xPixNp() const;
	
	int yPixNp() const;
	
	/**
	 * ================ 仅内部使用 ================
	 */
private:
	
	/**
	 * ~~~~~~~~~~~~ 成员函数 ~~~~~~~~~~~~
	 */
	
	Ui::GameDlg * ui;
	
	GamePanel * gp;
	NextPanel * np;
	
};

#endif // GAMEDLG_H









