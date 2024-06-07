#ifndef RESETDIALOG_H
#define RESETDIALOG_H

#include <QDialog>
#include "global.h"
#include <QMap>
namespace Ui {
class ResetDialog;
}

class ResetDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ResetDialog(QWidget *parent = nullptr);
    ~ResetDialog();
private:
    bool checkPassValid();
    void showTip(QString str,bool b_ok);
    bool checkEmailValid();
    bool checkVerifyValid();
    void addTipErr(TipErr te,QString tips);
    void delTipErr(TipErr te);
private slots:
    void on_return_btn_clicked();
    void on_sure_btn_clicked();
    void slotResetRep(int ok);
signals:
    void sigGoLogin();
private:

    Ui::ResetDialog *ui;
    QMap<TipErr, QString> _tip_errs;

};

#endif // RESETDIALOG_H
