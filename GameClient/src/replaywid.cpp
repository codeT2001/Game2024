#include "inc/replaywid.h"
#include "ui_replaywid.h"
#include "inc/sqlitemgr.h"
#include "inc/replyitem.h"
RePlayWid::RePlayWid(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RePlayWid)
{
    step = 0;
    ui->setupUi(this);
    ui->cd_widget->setIsReply(true);
    connect(ui->last_step_btn,&QPushButton::clicked,[&](){
        if(step == 0) return ;
        step--;
        ui->cd_widget->setStep(step);
    });
    connect(ui->next_setp_btn,&QPushButton::clicked,[&](){
        if(step == list.size()) return ;
        step++;
        ui->cd_widget->setStep(step);
    });
    initListWidget();
}

RePlayWid::~RePlayWid()
{
    delete ui;
}

void RePlayWid::initListWidget()
{
    SqliteMgr::getInstance().getChessGame(timelist);
    for (auto& str:timelist) {

        auto *replyitem = new ReplyItem(ui->replaylist);
        replyitem->setTitle(str);
        QListWidgetItem *item = new QListWidgetItem;
        item->setSizeHint(replyitem->sizeHint());
        ui->replaylist->addItem(item);
        ui->replaylist->setItemWidget(item,replyitem);
        connect(replyitem,&ReplyItem::clicked,[&](QString title){
            SqliteMgr::getInstance().loadChessGame(list,title);
                ui->cd_widget->setList(list);
                step = 0;
                ui->cd_widget->setStep(0);
        });
        ui->replaylist->addItem(item);
    }
}
