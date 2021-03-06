#include "GuideDlg.h"
#include "ui_GuideDlg.h"
#include "GameDlg.h"
#include "LoginDialog.h"
#include "../db/Usr.h"
#include <QMessageBox>

GuideDlg::GuideDlg(DbMgr & dbMgr, QWidget * parent/* = nullptr*/)
	: QDialog(parent), ui(new Ui::GuideDlg), m_dbMgr(dbMgr) {
	ui->setupUi(this);
	if(UsrData::MgrUsr != m_dbMgr->usr()->type()) {
		ui->mgBtn->hide();
	}
	connect(ui->newBtn, SIGNAL(clicked()), this, SLOT(onNewBtn()));
	connect(ui->mgBtn, SIGNAL(clicked()), this, SLOT(onMgBtn()));
	connect(ui->loadBtn, SIGNAL(clicked()), this, SLOT(onLoadBtn()));
	connect(ui->logoutBtn, SIGNAL(clicked()), this, SLOT(onLogoutBtn()));
	connect(ui->quitBtn, SIGNAL(clicked()), this, SLOT(onQuitBtn()));
}

GuideDlg::~GuideDlg() {
	delete ui;
}

void GuideDlg::onNewBtn() {
	close(); // 关闭本窗口
	GameDlg w(m_dbMgr, 1);
	w.exec();
}

void GuideDlg::onMgBtn() {
	QMessageBox::information(this, "提示", "不好意思！这个功能还没有做完！ : P");
}

void GuideDlg::onLoadBtn() {
	QMessageBox::StandardButton btn = QMessageBox::information(
				this, "提示",
				"不好意思！这个功能还没有做！ : P\n点击确定可直接进入第6关",
				QMessageBox::Ok | QMessageBox::Cancel);
	if(QMessageBox::Ok == btn) {
		close(); // 关闭本窗口
		GameDlg w(m_dbMgr, 6);
		w.exec();
	}
}

void GuideDlg::onLogoutBtn() {
	QMessageBox::StandardButton btn = QMessageBox::question(
				this, "提示", "是否退出登录？",
				QMessageBox::Ok | QMessageBox::Cancel);
	if(QMessageBox::Ok == btn) {
		close();
		LoginDialog w;
		w.exec();
	}
}

void GuideDlg::onQuitBtn() {
	QMessageBox::StandardButton btn = QMessageBox::question(
				this, "提示", "是否退出游戏？",
				QMessageBox::Ok | QMessageBox::Cancel);
	if(QMessageBox::Ok == btn) {
		
		close(); // 关闭窗口，退出程序
	}
}










