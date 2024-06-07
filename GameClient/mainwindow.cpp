#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "tcpmgr.h"
#include "player.h"
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    TcpMgr::getInstance().connectServer("127.0.0.1",50050);
    connect(&TcpMgr::getInstance(),&TcpMgr::sigConSuccess,this,&MainWindow::slotConSuccess);
    connect(&TcpMgr::getInstance(),&TcpMgr::sigGoGame,this,&MainWindow::slotGoGame);
    slotGoLogin();
    //slotGoGame();

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
    Player::getInstance().initFriendList();
    Player::getInstance().initApplyList();
    //Player::getInstance().setState(ONLINE);
    _game_dlg = new GameDialog(this);
    this->setFixedSize(_game_dlg->size());
    _game_dlg->setChessBoardEnabled(false);
    setCentralWidget(_game_dlg);
    connect(_game_dlg,&GameDialog::sigGoLogin,this,&MainWindow::slotGoLogin);
    connect(_game_dlg,&GameDialog::sigWidthChange,[&](){
        this->setFixedSize(_game_dlg->size());
    });
    //通知服务器成功登录
    QJsonObject obj;
    emit TcpMgr::getInstance().sigSendData(MSG_IDS::MSG_LOGIN_SUCCESS,obj);
}

void MainWindow::slotConSuccess(bool isCon)
{
    if(isCon)
    {
        qDebug()<<"connect success";
    }
    else
    {
        qDebug()<<"connect failed";
    }

}
