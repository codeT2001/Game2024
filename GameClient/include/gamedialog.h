#ifndef GAMEDIALOG_H
#define GAMEDIALOG_H

#include <QDialog>

namespace Ui {
class GameDialog;
}

class GameDialog : public QDialog
{
    Q_OBJECT

public:
    explicit GameDialog(QWidget *parent = nullptr);
    ~GameDialog();
    void setChessBoardEnabled(bool);
private slots:
    void on_go_login_btn_clicked();

signals:
    void sigGoLogin();

private:
    Ui::GameDialog *ui;
};

#endif // GAMEDIALOG_H
