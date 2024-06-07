#ifndef REPLAYWID_H
#define REPLAYWID_H

#include <QWidget>
#include <QList>
#include <QString>
#include "global.h"
namespace Ui {
class RePlayWid;
}
struct item{

};

class RePlayWid : public QWidget
{
    Q_OBJECT

public:
    explicit RePlayWid(QWidget *parent = nullptr);
    ~RePlayWid();
    void initListWidget();
private:
    Ui::RePlayWid *ui;
    QList<Item> list;
    QList<QString> timelist;
    int step;
};

#endif // REPLAYWID_H
