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
    bool checkUserValid();
    bool checkPassValid();
    void showTip(QString str,bool b_ok);
    bool checkEmailValid();
    bool checkVarifyValid();
    void addTipErr(TipErr te,QString tips);
    void delTipErr(TipErr te);
    //void initHandlers();
private slots:
    void on_return_btn_clicked();

    void on_varify_btn_clicked();

    //void slot_reset_mod_finish(ReqId id, QString res, ErrorCodes err);

    void on_sure_btn_clicked();

signals:
    void sigGoLogin();
private:

    Ui::ResetDialog *ui;
    QMap<TipErr, QString> _tip_errs;
    QMap<ReqId, std::function<void(const QJsonObject&)>> _handlers;

};

#endif // RESETDIALOG_H
