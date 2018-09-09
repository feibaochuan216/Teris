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
	
	void paintEvent(QPaintEvent * ev); // 绘制事件
	
	/**
	 * ~~~~~~~~~~~~ 底层实现 ~~~~~~~~~~~~
	 */
	
	void paint(const Lattice & ltc, QPainter & painter); // 绘制格子
	
	void paint(const Shape & shp, QPainter & painter); // 绘制形状
	
	void paintFaller(QPainter & painter); // 绘制下落形状
	
	void paintGp(QPainter & painter); // 绘制游戏面板
	
	void paintNp(QPainter & painter); // 绘制下一形状面板
	
	/**
	 * ================ 键盘事件处理 ================
	 */
	
	void keyPressEvent(QKeyEvent * ev); // 有按键按下时
	
	void keyReleaseEvent(QKeyEvent * ev); // 有按键弹起时
	
	/**
	 * ================ 槽 ================
	 */
public slots:
	void start(); // 点击开始按钮时
	
	void pause(); // 点击暂停按钮时
	
	void onFallTmOut(); // 下落计时器到时时
	
	void setDlg(); // 点击设置按钮时
	
	void exit(); // 点击退出按钮时
	
	/**
	 * ================ 动画效果 ================
	 */
	
	void gameOver(); // 游戏结束
	
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
	
	GamePanel * gp; // 游戏面板的业务逻辑
	NextPanel * np; // 下一形状面板的业务逻辑
};

#endif // GAMEDLG_H









