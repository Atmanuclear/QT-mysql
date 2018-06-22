#ifndef TIPSDIALOG_H
#define TIPSDIALOG_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class TipsDialog;
}

class TipsDialog : public QDialog
{
    Q_OBJECT

public:
    explicit TipsDialog(QWidget *parent = 0);
    ~TipsDialog();

private slots:
    void on_pushButton_clicked();

    void on_loginBtn_clicked();

private:
    Ui::TipsDialog *ui;
};

#endif // TIPSDIALOG_H
