#include "replaywid.h"
#include "ui_replaywid.h"
#include "sqlitemgr.h"
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
        QListWidgetItem *item = new QListWidgetItem(str, ui->replaylist);
        // 为每个项连接自定义槽函数
        connect(ui->replaylist, &QListWidget::itemClicked, this, [&]() {
            //去加载对局
            SqliteMgr::getInstance().loadChessGame(list,str);
            ui->cd_widget->setList(list);
        });
        ui->replaylist->addItem(item);
    }
}
