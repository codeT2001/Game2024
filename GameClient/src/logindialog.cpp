#include "inc/logindialog.h"
#include "ui_logindialog.h"
#include <QPainter>
#include <QPainterPath>
#include <QRegularExpression>
#include "inc/tcpmgr.h"
#include <QJsonObject>
LoginDialog::LoginDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginDialog)
{
    ui->setupUi(this);
    this->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);

    connect(ui->reg_btn,&QPushButton::clicked,[this](){
            emit sigGoRegister();
    });
    connect(ui->forget_label, &ClickedLabel::clicked, [this](){
        emit sigGoReset();
    });

    connect(ui->login_btn,&QPushButton::clicked,this,&LoginDialog::slotLoginClicked);

    ui->pass_edit->setEchoMode(QLineEdit::Password);

    ui->forget_label->setState("normal","hover","","selected","selected_hover","");

    initHead();
    connect(&TcpMgr::getInstance(),&TcpMgr::sigCantLogin,[&](){
        addTipErr(TipErr::TIP_LOGIN_FAILED,tr("登录失败"));
    });

}

LoginDialog::~LoginDialog()
{
    delete ui;
}

void LoginDialog::initHead()
{
    // 加载图片
    QPixmap originalPixmap(":/images/wzq.png");
        // 设置图片自动缩放
    qDebug()<< originalPixmap.size() << ui->head_label->size();
    originalPixmap = originalPixmap.scaled(ui->head_label->size(),
                                           Qt::KeepAspectRatio, Qt::SmoothTransformation);

    // 创建一个和原始图片相同大小的QPixmap，用于绘制圆角图片
    QPixmap roundedPixmap(originalPixmap.size());
    roundedPixmap.fill(Qt::transparent); // 用透明色填充

    QPainter painter(&roundedPixmap);
    painter.setRenderHint(QPainter::Antialiasing); // 设置抗锯齿，使圆角更平滑
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // 使用QPainterPath设置圆角
    QPainterPath path;
    path.addRoundedRect(0, 0, originalPixmap.width(), originalPixmap.height(), 10, 10); // 最后两个参数分别是x和y方向的圆角半径
    painter.setClipPath(path);

    // 将原始图片绘制到roundedPixmap上
    painter.drawPixmap(0, 0, originalPixmap);

    // 设置绘制好的圆角图片到QLabel上
    ui->head_label->setPixmap(roundedPixmap);
}

void LoginDialog::showTip(QString str, bool b_ok)
{
    if(b_ok){
        ui->err_tip->setProperty("state","normal");
    }else{
        ui->err_tip->setProperty("state","err");
    }

    ui->err_tip->setText(str);

    repolish(ui->err_tip);
}

bool LoginDialog::checkEmailValid()
{
    auto email = ui->email_edit->text();
    if(email.isEmpty())
    {
        qDebug()<<"email is Empty.";
        addTipErr(TipErr::TIP_EMAIL_ERR,tr("邮箱不能为空"));
        return false;
    }
    delTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool LoginDialog::checkPwdValid()
{
    auto pwd = ui->pass_edit->text();
    if(pwd.length() < 6 || pwd.length() > 15){
        qDebug() << "Pass length invalid";
        //提示长度不准确
        addTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
        return false;
    }

    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pwd).hasMatch();
    if(!match){
        //提示字符非法
        addTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符且长度为(6~15)"));
        return false;;
    }

    delTipErr(TipErr::TIP_PWD_ERR);

    return true;
}

bool LoginDialog::enableBtn(bool enabled)
{
    ui->login_btn->setEnabled(enabled);
    ui->reg_btn->setEnabled(enabled);
    return true;
}

void LoginDialog::addTipErr(TipErr te, QString tips)
{
    _tip_errs[te] = tips;
    showTip(tips, false);
}

void LoginDialog::delTipErr(TipErr tip)
{
    _tip_errs.remove(tip);
    if(_tip_errs.empty()){
        ui->err_tip->clear();
        return;
    }
    showTip(_tip_errs.first(), false);
}

void LoginDialog::slotLoginClicked()
{
    qDebug()<<"login btn clicked";
    //邮箱或者密码不正确
    if((!checkEmailValid()) || (!checkPwdValid()))
    {
        return ;
    }

    auto email = ui->email_edit->text();
    auto pwd = ui->pass_edit->text();

    QJsonObject jsonObj;
    jsonObj["email"] = email;
    jsonObj["passwd"] = xorString(pwd);
    emit TcpMgr::getInstance().sigSendData(MSG_IDS::MSG_LOGIN,jsonObj);
    delTipErr(TipErr::TIP_LOGIN_FAILED);
}

