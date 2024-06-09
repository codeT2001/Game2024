//#ifndef EMAIL_H
//#define EMAIL_H
//
//#include <QTcpSocket>
//#include <QtNetwork>
//class Email:public QObject
//{
//public:
//    static Email& getInstance() {
//        static Email instance;
//        return instance;
//    }
//    bool sendCode(QByteArray receiver, QByteArray body, QByteArray userName, QByteArray subject);
//    ~Email();
//private:
//    Email();
//    void waitingRead(bool);
//
//private:
//    QByteArray sender = "3308849335@qq.com"; 
//    QByteArray authcode = "xlxafczatqtkcjce";
//    QByteArray smtpServer = "smtp.qq.com";
//    int smtpPort = 465;
//    bool sendSuccess;
//    QByteArray recvdata;
//    QTcpSocket* socket;
//
//};
//
//#endif // EMAIL_H