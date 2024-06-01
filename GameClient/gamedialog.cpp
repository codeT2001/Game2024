#include "gamedialog.h"
#include "ui_gamedialog.h"

GameDialog::GameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::CustomizeWindowHint|
                         Qt::FramelessWindowHint);
}

GameDialog::~GameDialog()
{
    delete ui;
}

void GameDialog::setChessBoardEnabled(bool e)
{
    ui->chessboard_wid->setEnabled(e);
}

void GameDialog::on_go_login_btn_clicked()
{
    emit sigGoLogin();
}

