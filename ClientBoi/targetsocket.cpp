#include "targetsocket.h"

TargetSocket::TargetSocket(QString uuid,QObject *parent) : QObject(parent)
{
    targetSocket = new QTcpSocket(this);
    this->uuid = uuid;

    qDebug()<<"[debug][target] Target Socket with uuid:"<<uuid;
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
    qDebug()<<"[debug][info] Target Request Details:"<<request.left(100);
    //QByteArray requestArr = request.toUtf8();
    targetSocket->write(request.toUtf8().constData());
}

void TargetSocket::connected()
{
    qDebug() << "[debug][target] Connected to target successfully.";
}

void TargetSocket::disconnected()
{
    qDebug() << "[debug][target] Disconnected from target.";
    emit notifyDestroy();
}

void TargetSocket::bytesWritten(qint64 bytes)
{
    qDebug() <<"[debug][target]"<< bytes << " bytes written.";
}
void appendUUIDToArray(QByteArray& array,QString uuid){
    array.append(BLANK);
    array.append(BLANK);
    array.append(uuid);
}
void TargetSocket::readyRead()
{
    qDebug() << "[debug][target] Recieved data - reading";
    QByteArray response = targetSocket->readAll();
    //qDebug() << "[info][target]"<<response;
    //QByteArray respArr = response.toUtf8();
    QByteArray respArr = response;
    if(respArr.size()>=64000)
    {
        //greater than tcp socket read buffer size
        //so split into chunks
        QByteArray leftArr = respArr.left(64000);
        appendUUIDToArray(leftArr,uuid);
        emit replyReady(leftArr);
        QByteArray rightArr = respArr.right(respArr.size()-64000);
        appendUUIDToArray(rightArr,uuid);
        emit replyReady(rightArr);
    }
    else
    {
        appendUUIDToArray(respArr,uuid);
        emit replyReady(respArr);
    }
    qDebug()<<"[info][target] Attaching uuid to data:"<<uuid<<"\n"<<response;

}
