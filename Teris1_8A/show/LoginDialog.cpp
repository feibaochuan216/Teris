#include "logindialog.h"
#include "ui_logindialog.h"
#include <QPushButton>
#include <QDialogButtonBox>
#include "./show/GameDlg.h"
#include <QPalette>

LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    /*connect(ui->buttonBox,SIGNAL(accepted()),
            this,SLOT(on_buttonBox_accepted()));
    connect(ui->buttonBox,SIGNAL(rejected()),
            this,SLOT(on_buttonBox_rejected()));*/
    QPalette pe;
    pe.setColor(QPalette::WindowText, Qt::yellow);
    ui->label_2->setPalette(pe);
    ui->label_3->setPalette(pe);
}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::on_buttonBox_accepted()
{
    if(ui->UserEdit->text() == "csd1710" && ui->PasswdEdit->text() == "5211314"){
        qDebug("登录成功!");
        close();
        GameDlg w;
        w.exec();
    }
    else{
        QMessageBox msgbox(QMessageBox::Critical,windowTitle(),"用户名或密码有误！",QMessageBox::Ok,this);
        msgbox.setButtonText(QMessageBox::Ok,"确定");
        ui->UserEdit->setText("");
        ui->PasswdEdit->setText("");
        msgbox.exec();

    }
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
