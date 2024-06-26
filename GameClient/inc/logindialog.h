#ifndef LOGINDIALOG_H
#define LOGINDIALOG_H

#include <QDialog>
#include "global.h"
#include <QMap>
namespace Ui {
class LoginDialog;
}

class LoginDialog : public QDialog
{
    Q_OBJECT

public:
    explicit LoginDialog(QWidget *parent = nullptr);
    ~LoginDialog();
private:
    void initHead();
    void showTip(QString str,bool b_ok);
    bool checkEmailValid();
    bool checkPwdValid();
    bool enableBtn(bool);
    void addTipErr(TipErr te,QString tips);
    void delTipErr(TipErr te);

private slots:
    void slotLoginClicked();
signals:
    void sigGoRegister();
    void sigGoReset();
    void sigGoGame();
private:
    Ui::LoginDialog *ui;
    int _uid;
    QString _token;
    //QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;
    QMap<TipErr, QString> _tip_errs;
};

#endif // LOGINDIALOG_H
