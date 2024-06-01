#include "registerdialog.h"
#include "ui_registerdialog.h"
#include "global.h"
#include <QJsonDocument>
RegisterDialog::RegisterDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RegisterDialog),_countdown(5)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::CustomizeWindowHint|
                         Qt::FramelessWindowHint);


    ui->pass_edit->setEchoMode(QLineEdit::Password);
    ui->confirm_edit->setEchoMode(QLineEdit::Password);
    ui->err_tip->setProperty("state","normal");
    repolish(ui->err_tip);
//    connect(HttpMgr::getInstance().get(),&HttpMgr::sig_reg_mod_finish,
//            this,&RegisterDialog::slot_reg_mod_finish);

    //initHttpHandlers();
    ui->err_tip->clear();

    connect(ui->user_edit,&QLineEdit::editingFinished,this,[this](){
        checkUserValid();
    });
    connect(ui->email_edit, &QLineEdit::editingFinished, this, [this](){
        checkEmailValid();
    });
    connect(ui->pass_edit, &QLineEdit::editingFinished, this, [this](){
        checkPassValid();
    });
    connect(ui->confirm_edit, &QLineEdit::editingFinished, this, [this](){
        checkConfirmValid();
    });
    connect(ui->verify_edit, &QLineEdit::editingFinished, this, [this](){
        checkVarifyValid();
    });

    //设置浮动显示手形状
    ui->pass_visible->setCursor(Qt::PointingHandCursor);
    ui->confirm_visible->setCursor(Qt::PointingHandCursor);

    ui->pass_visible->setState("unvisible","unvisible_hover","","visible",
                               "visible_hover","");

    ui->confirm_visible->setState("unvisible","unvisible_hover","","visible",
                                  "visible_hover","");
    //连接点击事件
    connect(ui->pass_visible, &ClickedLabel::clicked, this, [this]() {
        auto state = ui->pass_visible->getCurState();
        if(state == ClickLbState::Normal){
            ui->pass_edit->setEchoMode(QLineEdit::Password);
        }else{
            ui->pass_edit->setEchoMode(QLineEdit::Normal);
        }
        qDebug() << "Label was clicked!";
    });

    connect(ui->confirm_visible, &ClickedLabel::clicked, this, [this]() {
        auto state = ui->confirm_visible->getCurState();
        if(state == ClickLbState::Normal){
            ui->confirm_edit->setEchoMode(QLineEdit::Password);
        }else{
            ui->confirm_edit->setEchoMode(QLineEdit::Normal);
        }
        qDebug() << "Label was clicked!";
    });

    // 创建定时器
    _countdown_timer = new QTimer(this);
    // 连接信号和槽
    connect(_countdown_timer, &QTimer::timeout, [this](){
        if(_countdown==0){
            _countdown_timer->stop();
            emit sigGoLogin();
            return;
        }
        _countdown--;
        auto str = QString("注册成功，%1 s后返回登录").arg(_countdown);
        ui->tip_lb->setText(str);
    });
}

RegisterDialog::~RegisterDialog()
{
    delete ui;
}

void RegisterDialog::on_get_code_clicked()
{
    qDebug()<<"clicked get_code button";
    auto email = ui->email_edit->text();
    //邮箱的正则表达式
    QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
    bool match = regex.match(email).hasMatch();
    if(match)
    {
        //发送邮箱给服务端，获取验证码
        qDebug()<<"发送邮箱给服务端，获取验证码";

    }
    else
    {
        showTip(tr("邮箱不正确"),false);
    }
}

//void RegisterDialog::slot_reg_mod_finish(ReqId id, QString res, ErrorCodes err)
//{
//    if(err != ErrorCodes::SUCCESS)
//    {
//        showTip(tr("网络请求错误"),false);
//        return ;
//    }
//    //解析JSON字符串，res转换为QByteArray
//    QJsonDocument jsonDoc = QJsonDocument::fromJson(res.toUtf8());
//    if(jsonDoc.isNull())
//    {
//        showTip(tr("json解析失败"),false);
//        return ;
//    }
//    //json 解析错
//    if(!jsonDoc.isObject())
//    {
//        showTip(tr("json解析失败"),false);
//        return ;
//    }
//    //json 解析成功
//    _handlers[id](jsonDoc.object());
//    return ;
//}

//void RegisterDialog::initHttpHandlers()
//{
//    //注册获取验证码回包的逻辑
//    _handlers[ReqId::ID_GET_VERIFY_CODE] = [this](const QJsonObject& jsonObj){
//        int error = jsonObj["error"].toInt();
//        if(error != ErrorCodes::SUCCESS)
//        {
//            showTip(tr("参数错误"),false);
//            return ;
//        }

//        auto email = jsonObj["email"].toString();
//        showTip(tr("验证码已经发送到邮箱，注意查收"),true);
//        qDebug()<<"email is "<<email;
//    };
//     _handlers[ReqId::ID_REG_USER]= [this](const QJsonObject& jsonObj){
//        int error = jsonObj["error"].toInt();
//        if(error != ErrorCodes::SUCCESS){
//            showTip(tr("参数错误"),false);
//            return;
//        }
//        auto email = jsonObj["email"].toString();
//        showTip(tr("用户注册成功"), true);
//        qDebug()<<"user uid is"<<jsonObj["uid"].toString();
//        qDebug()<< "email is " << email ;
//        ChangeTipPage();
//    };
//}

void RegisterDialog::showTip(QString str, bool b_ok)
{
    if(b_ok)
    {
        ui->err_tip->setProperty("state","normal");
    }
    else
    {
        ui->err_tip->setProperty("state","err");
    }
    ui->err_tip->setText(str);
    repolish(ui->err_tip);
}

void RegisterDialog::addTipErr(TipErr te, QString tips)
{
    _tip_errs[te] = tips;
    showTip(tips, false);
}

void RegisterDialog::delTipErr(TipErr te)
{
    _tip_errs.remove(te);
    if(_tip_errs.empty()){
        ui->err_tip->clear();
        return;
    }

    showTip(_tip_errs.first(), false);
}

bool RegisterDialog::checkUserValid()
{
    if(ui->user_edit->text() == ""){
        addTipErr(TipErr::TIP_USER_ERR, tr("用户名不能为空"));
        return false;
    }

    delTipErr(TipErr::TIP_USER_ERR);
    return true;
}

bool RegisterDialog::checkEmailValid()
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

bool RegisterDialog::checkPassValid()
{
    auto pass = ui->pass_edit->text();
    auto confirm = ui->confirm_edit->text();

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
        return false;
    }

    delTipErr(TipErr::TIP_PWD_ERR);

    if(pass != confirm){
        //提示密码不匹配
        addTipErr(TipErr::TIP_PWD_CONFIRM, tr("密码和确认密码不匹配"));
        return false;
    }else{
        delTipErr(TipErr::TIP_PWD_CONFIRM);
    }
    return true;
}

bool RegisterDialog::checkVarifyValid()
{
    auto pass = ui->verify_edit->text();
    if(pass.isEmpty()){
        addTipErr(TipErr::TIP_VERIFY_ERR, tr("验证码不能为空"));
        return false;
    }

    delTipErr(TipErr::TIP_VERIFY_ERR);
    return true;
}

bool RegisterDialog::checkConfirmValid()
{
    auto pass = ui->pass_edit->text();
    auto confirm = ui->confirm_edit->text();

    if(confirm.length() < 6 || confirm.length() > 15 ){
        //提示长度不准确
        addTipErr(TipErr::TIP_CONFIRM_ERR, tr("密码长度应为6~15"));
        return false;
    }

    // 创建一个正则表达式对象，按照上述密码要求
    // 这个正则表达式解释：
    // ^[a-zA-Z0-9!@#$%^&*]{6,15}$ 密码长度至少6，可以是字母、数字和特定的特殊字符
    QRegularExpression regExp("^[a-zA-Z0-9!@#$%^&*]{6,15}$");
    bool match = regExp.match(confirm).hasMatch();
    if(!match){
        //提示字符非法
        addTipErr(TipErr::TIP_CONFIRM_ERR, tr("不能包含非法字符"));
        return false;
    }

    delTipErr(TipErr::TIP_CONFIRM_ERR);

    if(pass != confirm){
        //提示密码不匹配
        addTipErr(TipErr::TIP_PWD_CONFIRM, tr("确认密码和密码不匹配"));
        return false;
    }else{
        delTipErr(TipErr::TIP_PWD_CONFIRM);
    }
    return true;
}

void RegisterDialog::changeTipPage()
{
    _countdown_timer->stop();
    ui->stackedWidget->setCurrentWidget(ui->page_2);

    // 启动定时器，设置间隔为1000毫秒（1秒）
    _countdown_timer->start(1000);
}

void RegisterDialog::on_sure_btn_clicked()
{
    bool valid = checkUserValid();
    if(!valid){
        return;
    }

    valid = checkEmailValid();
    if(!valid){
        return;
    }

    valid = checkPassValid();
    if(!valid){
        return;
    }

    valid = checkConfirmValid();
    if(!valid){
        return;
    }

    valid = checkVarifyValid();
    if(!valid){
        return;
    }
    qDebug() <<"发送验证码请求";
}

void RegisterDialog::on_return_btn_clicked()
{
    _countdown_timer->stop();
    emit sigGoLogin();
}

void RegisterDialog::on_cancel_btn_clicked()
{
    _countdown_timer->stop();
    emit sigGoLogin();
}

