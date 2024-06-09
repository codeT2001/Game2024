#ifndef GAMEDIALOG_H
#define GAMEDIALOG_H

#include <QDialog>
#include "global.h"
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
    void addUserList(); //测试
private slots:
    void slotMatchPlayer();
    void slotStartGame();
    void slotWin(COLOR c);
    void slotSendInfo();
    void slotRecvInfo(QString info);
    void slotCancleMatch();
    void slotGiveUp(bool f = true);
    void slotAddFriends();
    void slotShowApplys();
    void slotFlushApplyList();
    void slotFlushFriendList();
    void slotOpenChat(int pid);
    void slotSendChatInfo();
    void slotRecvChatInfo(int fid,const QString& chatName,const QString& info);
    void on_close_chat_btn_clicked();

signals:
    void sigGoLogin();
    void sigWidthChange();
private:
    void showSearch(const QString& str );
private:
    Ui::GameDialog *ui;
    bool m_loading;
    int m_chatId;

};

#endif // GAMEDIALOG_H
