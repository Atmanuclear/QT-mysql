#include "tipsdialog.h"
#include "ui_tipsdialog.h"

TipsDialog::TipsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TipsDialog)
{
    ui->setupUi(this);
}

TipsDialog::~TipsDialog()
{
    delete ui;
}

void TipsDialog::on_pushButton_clicked()
{
    QDialog *dlg = new QDialog(this);
        dlg->show();
}

void TipsDialog::on_loginBtn_clicked()
{
    if(ui->userlineEdit->text() == tr("003") &&
               ui->pwdlineEdit->text() == tr("123456"))
        {
           accept();
        } else {
           QMessageBox::warning(this, tr("Waring"),
                                 tr("user name or password error!"),
                                 QMessageBox::Yes);
        }

}

