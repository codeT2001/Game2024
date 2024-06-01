#ifndef CLICKEDLABEL_H
#define CLICKEDLABEL_H
#include "global.h"
#include <QLabel>

class ClickedLabel:public QLabel
{
    Q_OBJECT
public:
    ClickedLabel(QWidget* parent = nullptr);
    virtual void mousePressEvent(QMouseEvent *ev) override;
    virtual void mouseReleaseEvent(QMouseEvent *ev) override;
    virtual void enterEvent(QEnterEvent* event) override;
    virtual void leaveEvent(QEvent* event) override;
    void setState(QString normal="", QString hover="", QString press="",
                  QString select="", QString select_hover="", QString select_press="");

    ClickLbState getCurState();
    bool setCurState(ClickLbState state);
protected:

signals:
    void clicked(QString, ClickLbState);
private:
    QString _normal;
    QString _normal_hover;
    QString _normal_press;

    QString _selected;
    QString _selected_hover;
    QString _selected_press;

    ClickLbState _curstate;


};

#endif // CLICKEDLABEL_H
