#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    slotGoLogin();

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::slotGoReg()
{
    _reg_dlg = new RegisterDialog(this);
    this->setFixedSize(_reg_dlg->size());
    setCentralWidget(_reg_dlg);
    connect(_reg_dlg,&RegisterDialog::sigGoLogin,this,&MainWindow::slotGoLogin);
}

void MainWindow::slotGoLogin()
{
    _login_dlg = new LoginDialog(this);
    this->setFixedSize(_login_dlg->size());
    setCentralWidget(_login_dlg);
    connect(_login_dlg,&LoginDialog::sigGoRegister,this,&MainWindow::slotGoReg);
    connect(_login_dlg,&LoginDialog::sigGoReset,this,&MainWindow::slotGoReset);
    connect(_login_dlg,&LoginDialog::sigGoGame,this,&MainWindow::slotGoGame);
}

void MainWindow::slotGoReset()
{
    _reset_dlg = new ResetDialog(this);
    this->setFixedSize(_login_dlg->size());
    setCentralWidget(_reset_dlg);
    connect(_reset_dlg,&ResetDialog::sigGoLogin,this,&MainWindow::slotGoLogin);
}

void MainWindow::slotGoGame()
{
    _game_dlg = new GameDialog(this);
    this->setFixedSize(_game_dlg->size());
    _game_dlg->setChessBoardEnabled(false);
    setCentralWidget(_game_dlg);
    connect(_game_dlg,&GameDialog::sigGoLogin,this,&MainWindow::slotGoLogin);
    //connect(_login_dlg,&LoginDialog::switchRegister,this,&MainWindow::slotSwitchReg);
    //connect(_login_dlg,&LoginDialog::switchReset,this,&MainWindow::slotSwitchReset);
}
