#ifndef REPLYITEM_H
#define REPLYITEM_H

#include <QWidget>

namespace Ui {
class ReplyItem;
}

class ReplyItem : public QWidget
{
    Q_OBJECT

public:
    explicit ReplyItem(QWidget *parent = nullptr);
    ~ReplyItem();
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
    void setTitle(QString title );
signals:
    void clicked(QString);
private:
    Ui::ReplyItem *ui;
    QString m_title;
};

#endif // REPLYITEM_H
