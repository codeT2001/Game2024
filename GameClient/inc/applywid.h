#ifndef APPLYWID_H
#define APPLYWID_H

#include <QWidget>

namespace Ui {
class ApplyWid;
}

class ApplyWid : public QWidget
{
    Q_OBJECT

public:
    explicit ApplyWid(QWidget *parent = nullptr);
    ~ApplyWid();
    void setInfo(QString name,QString time);
signals:
    void sigDealApply(QString name,bool f);
private:
    Ui::ApplyWid *ui;
    QString m_name;
};

#endif // APPLYWID_H
