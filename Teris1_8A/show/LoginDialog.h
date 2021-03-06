#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include "db/DbMgr.h"
#include <QDialog>
#include <QPainter>
#include <QMessageBox>

namespace Ui {
	class LoginDialog;
}

/** 登录对话框：
 * @version 1.8A
 * @date 2018.09.11
 * @author 于延泽 */
class LoginDialog : public QDialog {
    Q_OBJECT

public:
    explicit LoginDialog(QWidget * parent = 0);
    ~LoginDialog();
	
	/**
	 * ================ 槽 ================
	 */

private slots:
    void on_buttonBox_accepted();

    void on_buttonBox_rejected();
	
	/**
	 * ================ 事件处理 ================
	 */

private:
    void paintEvent(QPaintEvent *);
	
	/**
	 * ================ 成员变量 ================
	 */
	
private:
    Ui::LoginDialog * ui;
	DbMgr m_dbMgr; // 数据库管理器
};

#endif // LOGINDIALOG_H







