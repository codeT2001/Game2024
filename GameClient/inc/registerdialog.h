#ifndef REGISTERDIALOG_H
#define REGISTERDIALOG_H

#include <QDialog>
#include "global.h"
#include <QTimer>
#include <QMap>
namespace Ui {
class RegisterDialog;
}

class RegisterDialog : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterDialog(QWidget *parent = nullptr);
    ~RegisterDialog();

private:
    void showTip(QString str, bool b_ok);
    void addTipErr(TipErr te,QString tips);
    void delTipErr(TipErr te);
    bool checkUserValid();
    bool checkEmailValid();
    bool checkPassValid();
    bool checkVerifyValid();
    bool checkConfirmValid();
    void changeTipPage();

private slots:
    void on_sure_btn_clicked();
    void on_return_btn_clicked();
    void on_cancel_btn_clicked();
    void slotRegisterRep(int ok);

signals:
    void sigGoLogin();
private:
    Ui::RegisterDialog *ui;
    QMap<TipErr, QString> _tip_errs;
    QTimer * _countdown_timer;
    int _countdown;
};

#endif // REGISTERDIALOG_H
