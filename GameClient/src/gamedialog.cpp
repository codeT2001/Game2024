#include "inc/gamedialog.h"
#include "ui_gamedialog.h"
#include "inc/tcpmgr.h"
#include <QJsonObject>
#include "inc/player.h"
#include <QMessageBox>
#include <QRandomGenerator>
#include "inc/userwid.h"
#include "inc/myinputdialog.h"
#include "inc/applywid.h"
#include <QDateTime>
GameDialog::GameDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GameDialog),m_loading(false)
{
    ui->setupUi(this);
    ui->send_btn_game->setEnabled(false);
    ui->cancle_match_btn->setEnabled(false);
    ui->giveup_btn->setEnabled(false);
    this->setWindowFlags(Qt::CustomizeWindowHint|Qt::FramelessWindowHint);
    connect(ui->match_btn,&QPushButton::clicked,this,&GameDialog::slotMatchPlayer);
    connect(ui->go_login_btn,&QPushButton::clicked,[&](){
       emit sigGoLogin();
    });
    ui->name_label->setText(Player::getInstance().m_name);
    connect(&TcpMgr::getInstance(),&TcpMgr::sigGameStart,this,&GameDialog::slotStartGame);
    connect(ui->chessboard_wid,&CDWidget::sigWin,this,&GameDialog::slotWin);
    connect(ui->send_btn_game,&QPushButton::clicked,this,&GameDialog::slotSendInfo);
    connect(&TcpMgr::getInstance(),&TcpMgr::sigRecvInfo,this,&GameDialog::slotRecvInfo);
    connect(ui->cancle_match_btn,&QPushButton::clicked,this,&GameDialog::slotCancleMatch);
    connect(ui->giveup_btn,&QPushButton::clicked,[&](){
        emit TcpMgr::getInstance().sigGiveUp(true);
    });
    connect(&TcpMgr::getInstance(),&TcpMgr::sigGiveUp,this,&GameDialog::slotGiveUp);
    ui->add_btn->SetState("normal","hover","press");
    connect(ui->search_edit,&CustomizeEdit::textChanged,[&](const QString& text){
        showSearch(text);

    });
    showSearch("");
    ui->chat_wid->hide();
    resize(width()-ui->chat_wid->width(), height());
    connect(ui->add_btn,&ClickedBtn::clicked,this,&GameDialog::slotAddFriends);
    connect(ui->apply_btn,&QPushButton::clicked,this,&GameDialog::slotShowApplys);
    connect(&Player::getInstance(),&Player::sigFlushApplyList,this,&GameDialog::slotFlushApplyList);
    connect(&Player::getInstance(),&Player::sigFlushFriendList,this,&GameDialog::slotFlushFriendList);
    connect(ui->chat_send_btn,&QPushButton::clicked,this,&GameDialog::slotSendChatInfo);
    connect(&TcpMgr::getInstance(),&TcpMgr::sigRecvChatInfo,this,&GameDialog::slotRecvChatInfo);
    ui->chat_text->setEnabled(false);
    connect(ui->replay_btn,&QPushButton::clicked,[&](){
        int x = ui->stackedWidget->currentIndex();
        if(x == 0)
        {
            //ui->vs_comp_btn->setEnabled(false);
            ui->match_btn->setEnabled(false);
            ui->cancle_match_btn->setEnabled(false);
            ui->giveup_btn->setEnabled(false);
            ui->replay_btn->setText(tr("返回主界面"));
        }
        else
        {
            //ui->vs_comp_btn->setEnabled(true);
            ui->match_btn->setEnabled(true);
            ui->cancle_match_btn->setEnabled(true);
            ui->giveup_btn->setEnabled(true);
            ui->replay_btn->setText(tr("回放"));
        }
        ui->stackedWidget->setCurrentIndex(1-x);
    });

}

GameDialog::~GameDialog()
{
    delete ui;
}

void GameDialog::setChessBoardEnabled(bool e)
{
    ui->chessboard_wid->setEnabled(e);
}

void GameDialog::slotMatchPlayer()
{
    //开始匹配，控制按钮
    QJsonObject obj;
    obj["pid"] = Player::getInstance().m_id;
    TcpMgr::getInstance().sigSendData(MSG_MATCH,obj);
    ui->match_btn->setEnabled(false);
    //ui->vs_comp_btn->setEnabled(false);
    ui->replay_btn->setEnabled(false);
    ui->cancle_match_btn->setEnabled(true);

}

void GameDialog::slotStartGame()
{
    QString str;
    if(Player::getInstance().m_color == COLOR::BLACK)
    {
        str = tr("你执黑棋,先手");
        ui->chessboard_wid->setEnabled(true);
        ui->chessboard_wid->setColor(COLOR::BLACK);
    }
    else
    {
        str = tr("你执白棋,后手");
        ui->chessboard_wid->setColor(COLOR::WHITE);
    }
    QMessageBox::about(this,tr("匹配成功"),str);
    ui->send_btn_game->setEnabled(true);
    ui->chessboard_wid->clearItems();
    ui->match_btn->setEnabled(false);
    ui->cancle_match_btn->setEnabled(false);
    ui->replay_btn->setEnabled(false);
    //ui->vs_comp_btn->setEnabled(false);
    ui->giveup_btn->setEnabled(true);
    ui->recv_text->clear();
}

void GameDialog::slotWin(COLOR c)
{
    QString str;
    if(c == COLOR::BLACK)
    {
       str = tr("黑棋胜利");
    }
    else
    {
        str = tr("白棋胜利");
    }
    QMessageBox::about(this,tr("游戏结束"),str);
    ui->send_btn_game->setEnabled(false);
    ui->match_btn->setEnabled(true);
    //ui->vs_comp_btn->setEnabled(true);
    ui->cancle_match_btn->setEnabled(false);
    ui->giveup_btn->setEnabled(false);
    ui->replay_btn->setEnabled(true);
    //发送消息，系统计算积分，更改状态
}

void GameDialog::slotSendInfo()
{
    QString info = ui->send_text->toPlainText();
    if(info.isEmpty())
    {
        return ;
    }
    QJsonObject obj;
    obj["rivalId"] = Player::getInstance().m_rivalId;
    obj["info"] = info;
    emit TcpMgr::getInstance().sigSendData(MSG_IDS::MSG_CHAT_GAME,obj);
    ui->recv_text->append(Player::getInstance().m_name+" : "+info);
    ui->send_text->clear();
}

void GameDialog::slotRecvInfo(QString info)
{
    ui->recv_text->append(Player::getInstance().m_rival+" : "+info);
}

void GameDialog::slotCancleMatch()
{
    //取消匹配
    QJsonObject obj;
    obj["pid"] = Player::getInstance().m_id;
    TcpMgr::getInstance().sigSendData(MSG_IDS::MSG_CANCEL_MATCH,obj);
    ui->send_btn_game->setEnabled(false);
    ui->cancle_match_btn->setEnabled(false);
    ui->giveup_btn->setEnabled(false);
    ui->match_btn->setEnabled(true);
    //ui->vs_comp_btn->setEnabled(true);
    ui->replay_btn->setEnabled(true);
}

void GameDialog::slotGiveUp(bool f)
{
    ui->chessboard_wid->setEnabled(false);
    COLOR c = Player::getInstance().m_color;
    if(f)   //自己认输，对方胜利,发送认输信号
    {
        QJsonObject obj;
        obj["rivalId"] = Player::getInstance().m_rivalId;
        emit TcpMgr::getInstance().sigSendData(MSG_GIVEUP,obj);
        c = Player::getInstance().r_color;
    }
     //对方认输
    emit ui->chessboard_wid->sigWin(c);



}

void GameDialog::slotAddFriends()
{
    MyInputDiaLog inputDlg;
    inputDlg.exec();

}

void GameDialog::slotShowApplys()
{
    slotFlushApplyList();
    ui->applyList->show();
    ui->chat_text->hide();
    ui->chat_send_btn->hide();
    ui->chat_eidt->hide();

    if(ui->chat_wid->isHidden())
    {
        ui->chat_wid->show();
        resize(width()+ui->chat_wid->width(), height());
        emit sigWidthChange();
    }

}

void GameDialog::slotFlushApplyList()
{
    //申请列表刷新
    ui->applyList->clear();
    foreach (const Apply* a, Player::getInstance().m_applyList) {
        auto *apply_wid = new ApplyWid(ui->applyList);
        apply_wid->setInfo(a->applyName,a->time);
        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(apply_wid->sizeHint());
        ui->applyList->addItem(item);
        ui->applyList->setItemWidget(item,apply_wid);
    }
}

void GameDialog::slotFlushFriendList()
{
    ui->user_list->clear();
    foreach (const Friends* f, Player::getInstance().m_friendsList) {
        auto *userwid = new UserWid(ui->user_list);
        connect(userwid,&UserWid::sigOpenChat,this,&GameDialog::slotOpenChat);
        userwid->setInfo(f->FriendId,f->name);
        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(userwid->sizeHint());
        ui->user_list->addItem(item);
        ui->user_list->setItemWidget(item,userwid);
    }
}

void GameDialog::slotOpenChat(int pid)
{
    m_chatId = pid;
    qDebug()<<pid;
    ui->chat_text->clear();
    for(const auto& info:Player::getInstance().m_chatMap[pid])
    {
        ui->chat_text->append(info);
    }
    ui->applyList->hide();
    ui->chat_text->show();
    ui->chat_send_btn->show();
    ui->chat_eidt->show();
    if(ui->chat_wid->isHidden())
    {
        ui->chat_wid->show();
        resize(width()+ui->chat_wid->width(), height());
        emit sigWidthChange();
    }

}

void GameDialog::slotSendChatInfo()
{
    QString info = ui->chat_eidt->text();
    if(info.isEmpty())
    {
        return ;
    }
    QJsonObject obj;
    obj["senderId"] = Player::getInstance().m_id;
    obj["chatId"] = m_chatId;
    obj["info"] = info;
    emit TcpMgr::getInstance().sigSendData(MSG_IDS::MSG_CHAT_INFO,obj);
    QString str = Player::getInstance().m_name+": "+info;
    ui->chat_text->append(str);
    ui->chat_eidt->clear();
    Player::getInstance().m_chatMap[m_chatId].append(str);
}

void GameDialog::slotRecvChatInfo(int fid, const QString& chatName,const QString &info)
{
    Player::getInstance().m_chatMap[fid].append(chatName + ": "+info);
    if(m_chatId == fid)
    {
        ui->chat_text->append(chatName + ": "+info);
    }
}

void GameDialog::showSearch(const QString& str )
{
    if(!str.isEmpty())
    {
        ui->user_list->hide();
        ui->search_list->show();
        ui->search_list->clear();
        foreach (const Friends* f, Player::getInstance().m_friendsList) {
            if(!f->name.contains(str)) continue;
            auto *userwid = new UserWid(ui->user_list);
            userwid->setInfo(f->FriendId,f->name);
            QListWidgetItem *item = new QListWidgetItem;
            item->setSizeHint(userwid->sizeHint());
            ui->search_list->addItem(item);
            ui->search_list->setItemWidget(item,userwid);
        }
    }
    else
    {
        ui->user_list->show();
        ui->search_list->hide();
    }
}

void GameDialog::on_close_chat_btn_clicked()
{
    ui->chat_wid->hide();
    resize(width()-ui->chat_wid->width(), height());
    emit sigWidthChange();
}

