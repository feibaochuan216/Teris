#include "SetDlg.h"
#include "ui_SetDlg.h"

SetDlg::SetDlg(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::SetDlg)
{
	ui->setupUi(this);
}

SetDlg::~SetDlg()
{
	delete ui;
}
