#include "inc/replyitem.h"
#include "ui_replyitem.h"

ReplyItem::ReplyItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ReplyItem)
{
    ui->setupUi(this);
}

ReplyItem::~ReplyItem()
{
    delete ui;
}
void ReplyItem::mouseReleaseEvent(QMouseEvent *event)
{

        emit clicked(m_title);

}

void ReplyItem::setTitle(QString title)
{
        m_title = title;
        ui->label->setText(title);
}
