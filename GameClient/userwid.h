#ifndef USERWID_H
#define USERWID_H

#include <QWidget>
#include "listitembase.h"
namespace Ui {
class UserWid;
}

class UserWid : public ListItemBase
{
    Q_OBJECT

public:
    explicit UserWid(QWidget *parent = nullptr);
    ~UserWid();
    QSize sizeHint() const override;
    void setInfo(int id,QString name/*QString level*/);

protected:
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
private slots:
    void slotShowContextMenu(const QPoint &pos);
signals:
    void sigOpenChat(int pid);
    void sigDelFriend(int pid,bool isSend);
    void sigWatchMsg();
    void sigInviteMsg();
private:
    Ui::UserWid *ui;
    int m_id;
    QString m_name;
    //QString m_level;
    //STATES m_state;
};

#endif // USERWID_H
