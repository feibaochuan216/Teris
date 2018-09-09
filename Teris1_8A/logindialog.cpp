#include "logindialog.h"
#include "ui_logindialog.h"
#include <QPushButton>
#include <QDialogButtonBox>
#include "./show/GameDlg.h"

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    /*connect(ui->buttonBox,SIGNAL(accepted()),
            this,SLOT(on_buttonBox_accepted()));
    connect(ui->buttonBox,SIGNAL(rejected()),
            this,SLOT(on_buttonBox_rejected()));*/
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_buttonBox_accepted()
{
    qDebug("登录成功!");
    close();
    GameDlg w;
    w.exec();


}
void LoginDialog::on_buttonBox_rejected()
{
    close();
}

void LoginDialog::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QRect rcImage = ui->frame->frameRect();
    rcImage.translate(ui->frame->pos());
    QImage image(":/picture/timg.jpg");
    painter.drawImage(rcImage,image);

}
