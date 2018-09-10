#ifndef GUIDEDLG_H
#define GUIDEDLG_H

#include "../db/DbMgr.h"
#include <QDialog>

namespace Ui {
	class GuideDlg;
}

/** 导航对话框：
 * 用户成功登录后进入的对话框，可新建游戏、加载存档等。
 */
class GuideDlg : public QDialog {
	Q_OBJECT
public:
	explicit GuideDlg(DbMgr & dbMgr, QWidget * parent = nullptr);
	~GuideDlg();
	
public slots:
	void onNewBtn(); // 新游戏按钮被点时
	
	void onMgBtn(); // 后台管理按钮被点时
	
	void onLoadBtn(); // 加载存档按钮被点时
	
	void onQuitBtn(); // 退出游戏按钮被点时
	
private:
	Ui::GuideDlg * ui;
	
	DbMgr m_dbMgr;
};

#endif // GUIDEDLG_H





