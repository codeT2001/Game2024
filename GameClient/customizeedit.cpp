#include "customizeedit.h"
#include "qdebug.h"
#include <QAction>
CustomizeEdit::CustomizeEdit(QWidget *parent):QLineEdit(parent)
{
    setMaxLength(8);
    setClearButtonEnabled(true);
    setPlaceholderText(tr("搜索"));
    QAction *action = new QAction(this);
    action->setIcon(QIcon(":/images/search.png"));
    addAction(action,QLineEdit::LeadingPosition);
}

void CustomizeEdit::focusOutEvent(QFocusEvent *event)
{
    QLineEdit::focusOutEvent(event);
    emit sigFocusOut();
}
