#include "GuideDlg.h"
#include "ui_GuideDlg.h"

GuideDlg::GuideDlg(QWidget * parent/* = nullptr*/) :
	QDialog(parent), ui(new Ui::GuideDlg) {
	ui->setupUi(this);
}

GuideDlg::~GuideDlg() {
	delete ui;
}





