#ifndef TARGETSOCKET_H
#define TARGETSOCKET_H

#include <QThread>
#include <QTcpSocket>
#include "constants.h"

class TargetSocket : public QThread
{
    Q_OBJECT
public:
    explicit TargetSocket(QString);
    void connectToTarget(QString,QString);
    void bridgeRequest(QString);
signals:
    void replyReady(QByteArray);
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
