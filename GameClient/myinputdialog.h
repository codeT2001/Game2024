#ifndef MYINPUTDIALOG_H
#define MYINPUTDIALOG_H
#include <QDialog>
#include <QLineEdit>
#include <QLabel>
class MyInputDiaLog:public QDialog
{
    Q_OBJECT
public:
   explicit MyInputDiaLog(QWidget *parent = nullptr);
private slots:
   void slotFindPlayer();
   void slotHasFind(bool f);
private:
   QLineEdit* userEdit;
   QLabel *tip;
};

#endif // MYINPUTDIALOG_H
