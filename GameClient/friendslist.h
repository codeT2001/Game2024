#ifndef FRIENDSLIST_H
#define FRIENDSLIST_H
#include <QWheelEvent>
#include <QListWidget>
#include <QEvent>
#include <QScrollBar>
class FriendsList:public QListWidget
{
    Q_OBJECT
public:
   explicit FriendsList(QWidget*parent = nullptr);

protected:
   virtual bool eventFilter(QObject *object, QEvent *event) override;

signals:
   void sigLoadingFriends();
};

#endif // FRIENDSLIST_H
