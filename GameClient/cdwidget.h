#ifndef CDWIDGET_H
#define CDWIDGET_H
#include <QObject>
#include <QWidget>
#include "global.h"


class CDWidget:public QWidget
{
    Q_OBJECT
public:
    explicit CDWidget(QWidget *parent = nullptr);
//    QVector<Item>& getItemList();
    void setColor(COLOR c);
    void clearItems();
    void setList(QList<Item>& list);
    void setIsReply(bool f);
    void setStep(int s);
protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *)override;
signals:
    void sigSendItem(Item item);
    void sigWin(COLOR c);
private:
    void drawChessboard();
     void drawItemsX();
    void drawItems();
    void drawItemWithMouse();                            //画棋子鼠标
    void drawChessAtPoint(QPainter& painter,QPoint& pt); //在棋盘上画棋子
    int countNearItem(Item, QPoint);
    bool isWin(Item);

private slots:
    void slotSendItem(Item item);
    void slotRecvItem(Item item);
private:
    //棋盘行、列数，以及每个单元格尺寸
    const int chessRows = 15;
    const int chessColumes = 15;
    const int rectWidth = 31;
    const int rectHeight = 31;
    const int dp[3] = {-1,0,1};
    COLOR m_color;
    QPoint m_lastPos;
    QVector<Item> m_items;        //棋子容器
    bool isReply = false;
    int m_step;
};

#endif // CDWIDGET_H
