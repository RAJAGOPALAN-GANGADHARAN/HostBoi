#include "networkbridge.h"
#include "targetsocket.h"

NetworkBridge::NetworkBridge(QObject *parent) : QObject(parent)
{
    socket = new QTcpSocket(this);

    connect(socket, SIGNAL(connected()),this, SLOT(connected()));
    connect(socket, SIGNAL(disconnected()),this, SLOT(disconnected()));
    connect(socket, SIGNAL(bytesWritten(qint64)),this, SLOT(bytesWritten(qint64)));
    connect(socket, SIGNAL(readyRead()),this, SLOT(readyRead()));

    basePrefix = QStringLiteral("/HostBoi/bridge");
    qDebug()<<"[debug] Created the Bridge socket";

    targetSocket = nullptr;

    //Test init
    targetHost="localhost";
    targetPort="1313";
    session="test";
}

void NetworkBridge::connectToServer(QString hostBoi)
{
    socket->connectToHost(hostBoi,80);
    if(!socket->waitForConnected(5000))
        qDebug()<<"[debug] Error:"<<socket->errorString();
}

void NetworkBridge::setBridgeTarget(QString host, QString port,QString sess)
{
    targetHost = host;
    targetPort = port;
    session = sess;
    qDebug()<<"[debug][client] Target host:"<<targetHost<<" ,Target port:"
        <<targetPort<<", Session:"<<sess;
}

void NetworkBridge::connected()
{
    QString fmt = QString("GET /HostBoi/connect/%1/ HTTP/1.1\r\n")
            .arg(session);
    socket->write(fmt.toStdString().c_str());
    qDebug() << "[debug] Connected to service successfully.";
}

void NetworkBridge::disconnected()
{
    qDebug() << "[debug][client] Disconnected from service.";
}

void NetworkBridge::bytesWritten(qint64 bytes)
{
    qDebug() <<"[debug][client]"<< bytes << " bytes written.";
}

void NetworkBridge::gotReply(QString reply)
{
    qDebug()<<"[debug][client] Sending reply back via bridge";
    socket->write(reply.toUtf8().constData());
}
QString extractUuidAndRemove(QString& data)
{
    static QString search=QString(BLANK)+QString(BLANK);
    int pos = data.lastIndexOf(search);
    QString uuid = data.right(data.size()-pos);

    data = data.left(pos);
    //qDebug()<<"[info][client] After removing uuid:"<<data;
    uuid.remove(0,2);
    qDebug()<<"[info][client] Extracted uuid:"<<uuid;
    return uuid;
}
void NetworkBridge::readyRead()
{
    qDebug() << "[debug][client] Recieved data - reading";
    QString request = socket->readAll();
    qDebug() << "[info][client]"<<request.left(100);
    if(!session.isEmpty() && !targetHost.isEmpty() && !targetPort.isEmpty()){
        QString uuid = extractUuidAndRemove(request);
        if(targetSocketMap.find(uuid) == targetSocketMap.end())
        {
            targetSocketMap[uuid] = new TargetSocket(uuid);
            connect(targetSocketMap[uuid],&TargetSocket::replyReady,
                    this,&NetworkBridge::gotReply);
            connect(targetSocketMap[uuid],&TargetSocket::notifyDestroy,
                    this,&NetworkBridge::targetDisconnected);
            targetSocketMap[uuid]->connectToTarget(targetHost,targetPort);
            qDebug()<<"[debug][client] Connection to target host sent";
        }
        targetSocketMap[uuid]->bridgeRequest(request);
        qDebug()<<"[debug][client] Request written to target host";
    }
}

void NetworkBridge::targetDisconnected()
{
    qDebug()<<"[debug][client] Target Disconnected - deleting";
}
