//#include "email.h"
//#include "iostream"
//Email::Email() :sender("3308849335@qq.com"), authcode("xlxafczatqtkcjce")
//, smtpServer("smtp.qq.com"), smtpPort(25), sendSuccess(true)
//{
//    socket = nullptr;
//}
//
//void Email::waitingRead(bool con)
//{
//    if (!con)
//    {
//        socket->waitForReadyRead(1000);
//    }
//    else
//    {
//        socket->waitForConnected(1000);
//    }
//    recvdata = socket->readAll();
//    int code = QString(recvdata).mid(0, 3).toInt();
//    std::cout << recvdata.toStdString() << std::endl;
//    std::cout << "recv code is " << code << std::endl;
//    if (code > 400)
//    {
//        sendSuccess = false;
//    }
//}
//
//bool Email::sendCode(QByteArray receiver, QByteArray body, QByteArray userName, QByteArray subject)
//{
//    socket = new QTcpSocket();
//
//    sendSuccess = true;
//    // 根据SSL使用情况选择端口并连接SMTP服务器
//    socket->connectToHost(smtpServer, 25, QTcpSocket::ReadWrite);
//    // 等待连接成功，超时2秒
//    waitingRead(true);
//
//    //建立SMTP通信
//    socket->write("HELO smtp.qq.com\r\n");
//    waitingRead(false);
//
//    socket->write("AUTH LOGIN\r\n");
//    waitingRead(false);
//
//    socket->write(sender.toBase64() + "\r\n");
//    waitingRead(false);
//
//    socket->write(authcode.toBase64() + "\r\n");
//    waitingRead(false);
//
//    socket->write("mail from:<" + sender + ">\r\n");
//    waitingRead(false);
//
//    socket->write("rcpt to:<"+receiver + ">\r\n");
//    waitingRead(false);
//    socket->write("data\r\n");
//    waitingRead(false);
//
//    //填写邮件信息
//    QByteArray fromName = "五子棋游戏官方";
//    QString message = "To:\""+userName+"\"<" + receiver + ">\r\n";
//    message += "From:\"" + fromName + "\"<" + sender + ">\r\n";
//    message += "Subject:" + subject + "\r\n";
//    message += "MIME-Version: 1.0\r\n";
//    message += "Content-Type: text/plain; charset=\"utf-8\"\r\n\r\n";
//    message += body + "\r\n";
//    std::cout << message.toStdString() << std::endl;
//    socket->write(message.toUtf8());
//    socket->write(".\r\n");
//    waitingRead(false);
//
//    //关闭连接
//    socket->write("quit\r\n");
//    waitingRead(false);
//    socket->close();
//    socket->deleteLater();
//    if(sendSuccess)
//        std::cout << "send success" << std::endl;
//    else
//        std::cout << "send failed" << std::endl;
//    return sendSuccess;
//
//}
//
//Email::~Email()
//{
//}
