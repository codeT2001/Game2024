#include "friendslist.h"


FriendsList::FriendsList(QWidget *parent):QListWidget(parent)
{
    //关闭横向滚动条和纵向滚动条
    setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    viewport()->installEventFilter(this);
}

bool FriendsList::eventFilter(QObject *object, QEvent *event)
{
    //检查事件是否是鼠标悬浮进入或离开
    if(object == this->viewport())
    {
        if(event->type() == QEvent::Enter)//显示滚动条
        {
            setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
        }
        else if(event->type() == QEvent::Leave)//隐藏滚动条
        {
            setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        }
    }
    //检查事件是否是滚轮事件
    if(object == viewport() && event->type() == QEvent::Wheel)
    {
        QWheelEvent *wheelEvent = static_cast<QWheelEvent*>(event);
        int numDegrees = wheelEvent->angleDelta().y()/8;
        int numSteps = numDegrees/15;   //计算滚动步数
        //设置滚动幅度
        verticalScrollBar()->setValue(verticalScrollBar()->value()-numSteps);

        //检查是否滚动到底部
        QScrollBar *scrollbar = verticalScrollBar();
        int maxScrollValue = scrollbar->maximum();
        int currentValue = scrollbar->value();

        if(maxScrollValue - currentValue <= 0)
        {
            emit sigLoadingFriends();
        }
        return true;
    }
    return QListWidget::eventFilter(object,event);
}
