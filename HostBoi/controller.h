#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QTcpServer>
#include <tcpthread.h>

class Controller : public QObject
{
    Q_OBJECT
public:
    explicit Controller(QObject *parent = nullptr);
    void startServer();
signals:

public slots:
    void handleNewConnection();
    void handleBridgeRequest(QString,TcpThread*);
    void handleBridgeMiddle(QString,QString,QTcpSocket*);
    void handleBridgeReply(QByteArray);

private:
    map<QString, TcpThread*> connectionMap;
    map<QString, QTcpSocket*> responseMap;
    map<QString, QString> idSessMap;
    QTcpServer* httpServer;
};

#endif // CONTROLLER_H
