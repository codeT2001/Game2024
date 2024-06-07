#include "applywid.h"
#include "ui_applywid.h"
#include <QDebug>
#include "player.h"
ApplyWid::ApplyWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ApplyWid)
{
    ui->setupUi(this);
    connect(ui->accpet_apply_btn,&QPushButton::clicked,[&](){
        emit sigDealApply(m_name,true);
    });
    connect(ui->refuse_btn,&QPushButton::clicked,[&](){
        emit sigDealApply(m_name,false);
    });
    connect(this,&ApplyWid::sigDealApply,&Player::getInstance(),&Player::slotDealApply);
}

ApplyWid::~ApplyWid()
{
    delete ui;
}

void ApplyWid::setInfo(QString name, QString time)
{
    m_name = name;
    ui->info_lb->setText(name + tr("申请添加你为好友"));
    ui->time_lb->setText(time);
}
