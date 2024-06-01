#ifndef CDWIDGET_H
#define CDWIDGET_H
#include <QObject>
#include <QWidget>
typedef struct Item{
    QPoint pt;
    bool black;
    bool operator ==(const Item& item) const
    {
        return pt == item.pt && black == item.black;
    }
}Item;


class CDWidget:public QWidget
{
    Q_OBJECT
public:
    explicit CDWidget(QWidget *parent = nullptr);

protected:
    void paintEvent(QPaintEvent *) override;
    void mousePressEvent(QMouseEvent *)override;

private:
    void drawChessboard();
    void drawItems();
    void drawItemWithMouse();                            //画棋子鼠标
    void drawChessAtPoint(QPainter& painter,QPoint& pt); //在棋盘上画棋子
    int countNearItem(Item, QPoint);
private:
    //棋盘行、列数，以及每个单元格尺寸
    const int chessRows = 15;
    const int chessColumes = 15;
    const int rectWidth = 30;
    const int rectHeight = 30;
    const int dp[3] = {-1,0,1};
    bool m_black;
    QPoint m_lastPos;
    QVector<Item> m_items;        //棋子容器
};

#endif // CDWIDGET_H
