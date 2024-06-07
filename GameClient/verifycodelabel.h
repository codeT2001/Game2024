#ifndef VERIFYCODELABEL_H
#define VERIFYCODELABEL_H
#include <QLabel>

class VerifyCodeLabel:public QLabel
{
    Q_OBJECT
public:
    explicit VerifyCodeLabel(QWidget *parent = nullptr);
    ~VerifyCodeLabel();
    QString getCode();
    void generateVerifyCode();
protected:
   virtual void paintEvent(QPaintEvent *e) override;
     virtual void mousePressEvent(QMouseEvent *ev) override;
signals:
   void clicked();
private:
    QString m_code;
    QVector<QRgb> m_colors;
    int m_codeNum {4}; // 验证码长度

};

#endif // VERIFYCODELABEL_H
