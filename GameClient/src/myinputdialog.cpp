#include "inc/myinputdialog.h"
#include <QDialogButtonBox>
#include "inc/tcpmgr.h"
#include <QFormLayout>
#include "inc/player.h"
MyInputDiaLog::MyInputDiaLog(QWidget *parent):QDialog(parent)
{

    setWindowTitle(tr("搜索玩家"));
    // 创建表单布局
    QFormLayout *layout = new QFormLayout(this);
    // 添加输入框
    userEdit = new QLineEdit(this);
    tip = new QLabel(this);
    layout->addRow(tip);
    layout->addRow(new QLabel(tr("请输入玩家名字:")), userEdit);
    tip->hide();
    // 添加按钮
    QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    layout->addWidget(buttonBox);

    // 连接信号和槽
    connect(buttonBox, &QDialogButtonBox::accepted, this, &MyInputDiaLog::slotFindPlayer);
    connect(buttonBox, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(&TcpMgr::getInstance(),&TcpMgr::sigHasFind,this,&MyInputDiaLog::slotHasFind);
}

void MyInputDiaLog::slotHasFind(bool f)
{
    if(!f)
    {
        tip->setText(tr("未找到该玩家，请重新输入"));
        tip->show();
    }
    else
    {
        tip->setText(tr("已发送添加信息"));
        tip->show();
    }
}

void MyInputDiaLog::slotFindPlayer()
{
    for(const auto& fri:Player::getInstance().m_friendsList)
    {
        if(fri->name == userEdit->text())
        {
            tip->setText("该玩家已是你的好友");
            return ;
        }
    }
    tip->setText(tr("正在寻找玩家,请耐心等待......"));
    tip->show();
    QString username = userEdit->text();
    QJsonObject obj;
    obj["name"] = username;
    TcpMgr::getInstance().sigSendData(MSG_IDS::MSG_ADD_PLAYER,obj);
}

