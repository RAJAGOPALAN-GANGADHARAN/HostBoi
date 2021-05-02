#ifndef NETWORKBRIDGE_H
#define NETWORKBRIDGE_H

#include <QObject>
#include <QTcpSocket>
#include "constants.h"

class TargetSocket;

class NetworkBridge : public QObject
{
    Q_OBJECT
public:
    explicit NetworkBridge(QObject *parent = nullptr);
    void connectToServer(QString);
    void setBridgeTarget(QString,QString,QString);
signals:

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();
    void gotReply(QString);
    void targetDisconnected();

private:
    QTcpSocket *socket;
    TargetSocket* targetSocket;
    QString targetHost;
    QString targetPort;
    QString session;
    QString basePrefix;
};
#endif // NETWORKBRIDGE_H
