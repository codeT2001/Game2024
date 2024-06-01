#include "cdwidget.h"
#include <QMouseEvent>
#include <QPainter>
#include <QMessageBox>
CDWidget::CDWidget(QWidget *parent):QWidget(parent)
{
    m_black = true;
    m_lastPos = QPoint(-1,-1);
    this->setMinimumSize((chessColumes + 1) * rectWidth, (chessRows + 1) * rectHeight);
    this->setMaximumSize((chessColumes + 1) * rectWidth, (chessRows + 1) * rectHeight);
}

void CDWidget::paintEvent(QPaintEvent *)
{
    drawChessboard();       //画棋盘
    drawItems();            //画棋子
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
            return ;
        }
    }

    m_lastPos = pt;
    Item item{pt,m_black};
    m_items.append(item);

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
            //胜利
            QString str = m_black? QStringLiteral("黑棋胜利！") : QStringLiteral("白棋胜利！");
            QMessageBox::information(this, QStringLiteral("游戏结束"),str, QMessageBox::Yes , QMessageBox::Yes);
            m_items.clear();
            return;
        }
    }


    m_black = !m_black;  //棋子换颜色
    }
    QWidget::mousePressEvent(e);


}

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

void CDWidget::drawItems()
{
    QPainter painter(this);                               //创建画家
    painter.setPen(QPen(QColor(Qt::transparent)));        //设置画笔透明

    foreach(auto item,m_items) {
        if (item.pt == m_lastPos) {                     //棋子坐标 == 最后落下的棋子的坐标
            painter.setPen(QPen(QColor("#ff3333"),2,Qt::SolidLine)); //画笔，红边
            if (item.black) {
                painter.setBrush(Qt::black);                         //黑色设置黑画刷
            }
            else {
                painter.setBrush(Qt::white);                         //白色设置白画刷
            }
            drawChessAtPoint(painter,item.pt);                      //在棋盘上画棋子
            painter.setPen(QPen(QColor(Qt::transparent)));           //设置画笔透明
            continue;
        }
        if (item.black) {                   //非最后落下的棋子，没有红边
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

    if (m_black    /*mClient->IsBlack()*/) {                        //客户端返回棋子颜色
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
