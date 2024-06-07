#include "verifycodelabel.h"
#include <QPainter>
#include <QPaintEvent>
#include <QFont>
#include <QColor>
#include <QRandomGenerator>
#include <qdebug>
#include <QMouseEvent>
VerifyCodeLabel::VerifyCodeLabel(QWidget *parent)
    :QLabel(parent)
{
    this->clear();
    generateVerifyCode();
    connect(this,&VerifyCodeLabel::clicked,this,[&](){
        generateVerifyCode();
    });
}

VerifyCodeLabel::~VerifyCodeLabel()
{

}

QString VerifyCodeLabel::getCode()
{
    return m_code;
}

void VerifyCodeLabel::generateVerifyCode()
{
    m_code = QString();
    for (int i = 0; i < m_codeNum; i++) {
        int flag = QRandomGenerator::global()->bounded(2);
        if (0 == flag) {
            int c = '0' + QRandomGenerator::global()->bounded(10);
            m_code += static_cast<QChar>(c);
        }
        else {
            int c = (QRandomGenerator::global()->bounded(2)) ? 'a' : 'A';
            m_code += static_cast<QChar>(c + QRandomGenerator::global()->bounded(26));
        }
    }
    m_colors.clear();
    for (int i = 0; i < 4; i++)
    {
        m_colors.append(qRgb(2 + QRandomGenerator::global()->bounded(16),
                             2 + QRandomGenerator::global()->bounded(16),
                             2 + QRandomGenerator::global()->bounded(16)));
    }
    update(); // 触发重绘
}

void VerifyCodeLabel::paintEvent(QPaintEvent *e)
{

    QPainter painter(this);
    painter.fillRect(rect(), Qt::white); // 清除原有内容并填充背景色
    painter.setFont(QFont("Comic Sans MS", 12));

    for (int i = 0; i < m_codeNum; i++)
    {
        painter.setPen(QPen(m_colors[i]));
        painter.drawText(20 * i, 0, 20, 30, Qt::AlignCenter, QString(m_code[i]));
    }
    for (int i = 0; i < 150; i++)
    {
        painter.setPen(QPen(QColor::fromRgba(m_colors[i % m_colors.size()])));
        painter.drawPoint(QRandomGenerator::global()->bounded(width()), QRandomGenerator::global()->bounded(height()));
    }
    QLabel::paintEvent(e);
}

void VerifyCodeLabel::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        emit clicked();
    }
    QLabel::mousePressEvent(event);
}
