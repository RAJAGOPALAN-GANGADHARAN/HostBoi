#ifndef TARGETSOCKET_H
#define TARGETSOCKET_H

#include <QObject>
#include <QTcpSocket>
#include "constants.h"

class TargetSocket : public QObject
{
    Q_OBJECT
public:
    explicit TargetSocket(QString,QObject *parent = nullptr);
    void connectToTarget(QString,QString);
    void bridgeRequest(QString);
signals:
    void replyReady(QString);
    void notifyDestroy();

public slots:
    void connected();
    void disconnected();
    void bytesWritten(qint64 bytes);
    void readyRead();

private:
    QTcpSocket* targetSocket;
    QString uuid;
};

#endif // TARGETSOCKET_H
