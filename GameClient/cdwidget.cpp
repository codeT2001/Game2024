#include "cdwidget.h"
#include <QMouseEvent>
#include <QPainter>
#include <QMessageBox>
#include "player.h"
#include "tcpmgr.h"
#include "sqlitemgr.h"
#include "QTimer"
CDWidget::CDWidget(QWidget *parent):QWidget(parent)
{
    m_color = COLOR::BLACK;
    m_lastPos = QPoint(-1,-1);
    this->setEnabled(false);
    this->setMinimumSize((chessColumes + 1) * rectWidth, (chessRows + 1) * rectHeight);
    this->setMaximumSize((chessColumes + 1) * rectWidth, (chessRows + 1) * rectHeight);
    connect(this,&CDWidget::sigSendItem,this,&CDWidget::slotSendItem);
    connect(&TcpMgr::getInstance(),&TcpMgr::sigRecvItem,this,&CDWidget::slotRecvItem);
    QTimer *timer = new QTimer(this);
    connect(timer,&QTimer::timeout,[&](){
        update();
    });
    timer->start(100);
}

void CDWidget::setColor(COLOR c)
{
    m_color = c;
}

void CDWidget::clearItems()
{
    m_items.clear();
}

void CDWidget::setList(QList<Item> &list)
{
    m_items = list;
}

void CDWidget::setIsReply(bool f)
{
    isReply = f;
}

void CDWidget::setStep(int s)
{
    m_step = s;
    update();
}
void CDWidget::paintEvent(QPaintEvent *)
{
    drawChessboard();       //画棋盘
    if(isReply)
    {
        drawItemsX();
    }
    else
    {
        drawItems();            //画棋子
    }
    drawItemWithMouse();    //画棋子鼠标
    update();
}

void CDWidget::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        QPoint pt;
        pt.setX(e->pos().x()/rectWidth);
        pt.setY(e->pos().y()/rectHeight);

        foreach(auto& item, m_items)
        {
            if(item.pt == pt)
            {
                QWidget::mousePressEvent(e);
                return ;
            }
        }

        m_lastPos = pt;
        Item item{pt,m_color};
        m_items.append(item);
        update();
        emit sigSendItem(item);
        bool f = isWin(item);//判断自己是否胜利
        if(f)//如果胜利了
        {
            //禁止棋盘操作
            this->setEnabled(false);
            emit sigWin(m_color);
            //Player::getInstance().setState(ONLINE);
            SqliteMgr::getInstance().saveChessGame(m_items);
        }
    }
    QWidget::mousePressEvent(e);
}

//画棋盘
void CDWidget::drawChessboard()
{
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing,true);
    painter.setBrush(Qt::darkYellow);
    painter.setPen(QPen(QColor(Qt::black),2));

    for(int i=0;i<chessColumes;i++)
    {
        for(int j = 0; j<chessRows; j++)
        {
            painter.drawRect((i+0.5)*rectWidth,(j+0.5)*rectHeight,rectWidth,rectHeight);
        }
    }
}


//画棋子
void CDWidget::drawItems()
{
    QPainter painter(this);                               //创建画家
    painter.setPen(QPen(QColor(Qt::transparent)));        //设置画笔透明

    foreach(auto item,m_items) {
        if (item.pt == m_lastPos) {                     //棋子坐标 == 最后落下的棋子的坐标
            painter.setPen(QPen(QColor("#ff3333"),2,Qt::SolidLine)); //画笔，红边
            if (item.color==COLOR::BLACK) {
                painter.setBrush(Qt::black);                         //黑色设置黑画刷
            }
            else {
                painter.setBrush(Qt::white);                         //白色设置白画刷
            }
            drawChessAtPoint(painter,item.pt);                      //在棋盘上画棋子
            painter.setPen(QPen(QColor(Qt::transparent)));           //设置画笔透明
            continue;
        }
        if (item.color==COLOR::BLACK) {                   //非最后落下的棋子，没有红边
            painter.setBrush(Qt::black);
        }
        else {
            painter.setBrush(Qt::white);
        }
        drawChessAtPoint(painter,item.pt); //在棋盘上画棋子
    }
}

void CDWidget::drawItemsX()
{
    QPainter painter(this);                               //创建画家
    painter.setPen(QPen(QColor(Qt::transparent)));        //设置画笔透明

    for(int i=0;i<m_step && i<m_items.size();++i) {
        auto item = m_items.at(i);
        if (i == m_step -1) {                     //棋子坐标 == 最后落下的棋子的坐标
            painter.setPen(QPen(QColor("#ff3333"),2,Qt::SolidLine)); //画笔，红边
            if (item.color==COLOR::BLACK) {
                painter.setBrush(Qt::black);                         //黑色设置黑画刷
            }
            else {
                painter.setBrush(Qt::white);                         //白色设置白画刷
            }
            drawChessAtPoint(painter,item.pt);                      //在棋盘上画棋子
            painter.setPen(QPen(QColor(Qt::transparent)));           //设置画笔透明
            continue;
        }
        if (item.color==COLOR::BLACK) {                   //非最后落下的棋子，没有红边
            painter.setBrush(Qt::black);
        }
        else {
            painter.setBrush(Qt::white);
        }
        drawChessAtPoint(painter,item.pt); //在棋盘上画棋子
    }
}

void CDWidget::drawItemWithMouse()
{
    QPainter painter(this);                         //创建画家
    painter.setPen(QPen(QColor(Qt::transparent)));  //设置画笔透明
    if (m_color == COLOR::BLACK) {                        //客户端返回棋子颜色
        painter.setBrush(Qt::black);
    }
    else {
        painter.setBrush(Qt::white);
    }
    //光标坐标绘制成棋子摸样
    painter.drawEllipse(mapFromGlobal(QCursor::pos()), rectWidth / 2, rectHeight / 2);  //全局坐标系下的点转换为相对于对象本身坐标系下的点
}

void CDWidget::drawChessAtPoint(QPainter &painter, QPoint &pt)
{
    QPoint ptCenter((pt.x()+0.5)*rectWidth,(pt.y()+0.5)*rectHeight);  //棋子坐标（0,0）转换成在棋盘上的坐标（）
    painter.drawEllipse(ptCenter,rectWidth / 2,rectHeight / 2);       //画圆形棋子，圆心半径
}

int CDWidget::countNearItem(Item item, QPoint pt)
{
    int cnt = 0;
    item.pt += pt;
    while(m_items.contains(item))
    {
        cnt++;
        item.pt += pt;
    }
    return cnt;
}

bool CDWidget::isWin(Item item)
{
    int cnt[9] = {0};
    for(int i = 0 ;i < 3; i++)
    {
            for(int j = 0; j < 3 ; j++)
            {
                int idx = 3*i+j;
                if(idx == 4) continue;
                cnt[idx] = countNearItem(item,QPoint(dp[i],dp[j]));
            }
        }

        for(int i=0;i<4;i++)
        {
            if(cnt[i]+cnt[8-i]>=4)
            {
                return true;
            }
        }
        return false;
}

void CDWidget::slotSendItem(Item item)
{
    this->setEnabled(false);
    int x = item.pt.x();
    int y = item.pt.y();
    QJsonObject obj;
    obj["posx"] = x;
    obj["posy"] = y;
    obj["color"] = m_color;
    obj["rivalId"] = Player::getInstance().m_rivalId;
    TcpMgr::getInstance().sigSendData(MSG_IDS::MSG_MOVE,obj);
}

void CDWidget::slotRecvItem(Item item)
{
    m_items.append(item);
    update();
    m_lastPos = item.pt;
    bool f = isWin(item);   //判断对方是否胜利
    if(f)
    {
        this->setEnabled(false);
        emit sigWin(item.color);
        SqliteMgr::getInstance().saveChessGame(m_items);
        //Player::getInstance().setState(ONLINE);
    }
    else
    {
        this->setEnabled(true);
    }

}
