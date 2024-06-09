#ifndef CUSTOMIZEEDIT_H
#define CUSTOMIZEEDIT_H

#include <QLineEdit>
class CustomizeEdit:public QLineEdit
{
    Q_OBJECT
public:
    explicit CustomizeEdit(QWidget *parent = nullptr);
protected:
    virtual void focusOutEvent(QFocusEvent *event) override;
signals:
    void sigFocusOut();
};

#endif // CUSTOMIZEEDIT_H
