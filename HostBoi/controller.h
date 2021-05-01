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

private:
    map<QString, TcpThread*> connectionMap;
    QTcpServer* httpServer;
};

#endif // CONTROLLER_H
