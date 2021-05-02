#include "targetsocket.h"

TargetSocket::TargetSocket(QString uuid,QObject *parent) : QObject(parent)
{
    targetSocket = new QTcpSocket(this);
    this->uuid = uuid;

    connect(targetSocket, SIGNAL(connected()),this, SLOT(connected()));
    connect(targetSocket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(targetSocket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(targetSocket, SIGNAL(readyRead()),this, SLOT(readyRead()));

}

void TargetSocket::connectToTarget(QString host,QString port)
{
    targetSocket->connectToHost(host,port.toInt());
    while(!targetSocket->waitForConnected(5000))
        qDebug()<<"[debug][target] Error connecting to target:"<<targetSocket->errorString();
}

void TargetSocket::bridgeRequest(QString request)
{
    qDebug()<<"[debug][target] Sending request to target!";
    qDebug()<<"[debug][info] Target Request Details:"<<request;
    targetSocket->write(request.toUtf8().constData());
}

void TargetSocket::connected()
{
    qDebug() << "[debug][target] Connected to target successfully.";
}

void TargetSocket::disconnected()
{
    qDebug() << "[debug][target] Disconnected from target.";
}

void TargetSocket::bytesWritten(qint64 bytes)
{
    qDebug() <<"[debug][target]"<< bytes << " bytes written.";
}

void TargetSocket::readyRead()
{
    qDebug() << "[debug][target] Recieved data - reading";
    QString response = targetSocket->readAll();
    qDebug() << "[info][target]"<<response;

    //append uuid
    response.append(BLANK);
    response.append(BLANK);
    response.append(uuid);

    emit replyReady(response);
}
