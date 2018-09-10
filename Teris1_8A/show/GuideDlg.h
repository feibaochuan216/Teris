#ifndef GUIDEDLG_H
#define GUIDEDLG_H

#include <QDialog>

namespace Ui {
	class GuideDlg;
}

class GuideDlg : public QDialog {
	Q_OBJECT
	
public:
	explicit GuideDlg(QWidget * parent = nullptr);
	~GuideDlg();
	
private:
	Ui::GuideDlg * ui;
};

#endif // GUIDEDLG_H





