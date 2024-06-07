#include "resetdialog.h"
#include "ui_resetdialog.h"
#include <QDebug>
#include <QRegularExpression>
#include "global.h"
#include <QJsonDocument>
#include <QRegularExpressionValidator>
#include "tcpmgr.h"
#include <QMessageBox>
ResetDialog::ResetDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ResetDialog)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    ui->verify_edit->setMaxLength(4);
    // 创建正则表达式
    QRegularExpression regex("^[A-Za-z0-9]+$");
    // 创建验证器
    QRegularExpressionValidator *validator = new QRegularExpressionValidator(regex);
    // 应用验证器
    ui->verify_edit->setValidator(validator);


    connect(ui->email_edit, &QLineEdit::editingFinished, [this](){
        checkEmailValid();
    });

    connect(ui->pwd_edit, &QLineEdit::editingFinished,  [this](){
        checkPassValid();
    });

    connect(ui->verify_edit, &QLineEdit::editingFinished, [this](){
         checkVerifyValid();
    });
    connect(&TcpMgr::getInstance(),&TcpMgr::sigResetRep,this,&ResetDialog::slotResetRep);
}

ResetDialog::~ResetDialog()
{
    delete ui;
}

void ResetDialog::on_return_btn_clicked()
{
    qDebug() << "return btn clicked ";
    emit sigGoLogin();
}

bool ResetDialog::checkPassValid()
{
    auto pass = ui->pwd_edit->text();

    if(pass.length() < 6 || pass.length()>15){
        //提示长度不准确
        addTipErr(TipErr::TIP_PWD_ERR, tr("密码长度应为6~15"));
        return false;
    }

    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(pass).hasMatch();
    if(!match){
        //提示字符非法
        addTipErr(TipErr::TIP_PWD_ERR, tr("不能包含非法字符"));
        return false;;
    }

    delTipErr(TipErr::TIP_PWD_ERR);

    return true;
}

bool ResetDialog::checkEmailValid()
{
    //验证邮箱的地址正则表达式
    auto email = ui->email_edit->text();
    // 邮箱地址的正则表达式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch(); // 执行正则表达式匹配
    if(!match){
        //提示邮箱不正确
        addTipErr(TipErr::TIP_EMAIL_ERR, tr("邮箱地址不正确"));
        return false;
    }

    delTipErr(TipErr::TIP_EMAIL_ERR);
    return true;
}

bool ResetDialog::checkVerifyValid()
{
    auto pass = ui->verify_edit->text();
    int n = pass.compare(ui->code_label->getCode(),Qt::CaseInsensitive);
    if(pass.isEmpty() || n!=0){
        addTipErr(TipErr::TIP_VERIFY_ERR, tr("验证码错误"));
        return false;
    }
    delTipErr(TipErr::TIP_VERIFY_ERR);
    return true;
}

void ResetDialog::addTipErr(TipErr te, QString tips)
{
    _tip_errs[te] = tips;
    showTip(tips, false);
}

void ResetDialog::delTipErr(TipErr te)
{
    _tip_errs.remove(te);
    if(_tip_errs.empty()){
      ui->err_tip->clear();
      return;
    }

    showTip(_tip_errs.first(), false);
}

void ResetDialog::showTip(QString str, bool b_ok)
{
    if(b_ok){
         ui->err_tip->setProperty("state","normal");
    }else{
        ui->err_tip->setProperty("state","err");
    }

    ui->err_tip->setText(str);

    repolish(ui->err_tip);
}

void ResetDialog::on_sure_btn_clicked()
{

    bool valid = checkEmailValid();
    if(!valid){
        return;
    }

    valid = checkPassValid();
    if(!valid){
        return;
    }

    valid = checkVerifyValid();
    if(!valid){
        return;
    }

    //发送TCP重置用户请求
    qDebug()<<"发送TCP重置用户请求";
    QJsonObject obj;
    obj["email"] = ui->email_edit->text();
    obj["newpwd"] = xorString(ui->pwd_edit->text());
    emit TcpMgr::getInstance().sigSendData(MSG_IDS::MSG_RESET,obj);
}

void ResetDialog::slotResetRep(int ok)
{
    if(ok == 1)
    {
        QMessageBox::about(this,tr("重置密码"),tr("重置密码成功！"));
        emit sigGoLogin();
    }
    else
    {
        QMessageBox::about(this,tr("重置密码"),tr("重置密码失败！"));
    }
}

