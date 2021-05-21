#include "controller.h"
#include "handlers.h"
#include <QUuid>

Controller::Controller(QObject *parent) : QObject(parent)
{
    httpServer = new QTcpServer(this);
    TcpThread::registerHandles("^/home$",homeRequest);
    TcpThread::registerHandles("/404",badRequest);
    TcpThread::registerHandles("^/HostBoi/create$",createRequest);
    TcpThread::registerHandles("^/HostBoi/connect/[a-zA-Z0-9_.-]*/$",bridgeConnectService);
    TcpThread::registerHandles("^/HostBoi/bridge/[a-zA-Z0-9_.-]*/",bridgeService);
}

void Controller::startServer()
{
    //Connect slots
    connect(httpServer,&QTcpServer::newConnection,
            this,&Controller::handleNewConnection);

    httpServer->listen(QHostAddress::Any, 80);
    //httpServer->listen(QHostAddress("hostboi"),80);
    qDebug()<<"[debug] Attempting to start a server at port 80";
    if(httpServer->isListening())
        qDebug()<<"[debug] Server started successfully on port 80";
    else
        qDebug()<<"[debug] Server couldn't start on port 80";

}

void Controller::handleNewConnection()
{
    qDebug()<<"[debug] Incoming Connection";
    TcpThread* connectionThread = new TcpThread(httpServer->nextPendingConnection());
    connect(connectionThread,&TcpThread::isBridgeRequest,
            this,&Controller::handleBridgeRequest);
    connect(connectionThread,&TcpThread::bridgeRequest,
            this,&Controller::handleBridgeMiddle);
    connect(connectionThread,&TcpThread::bridgeReply,
            this,&Controller::handleBridgeReply);

    connectionThread->start();

    qDebug()<<"[debug] Incoming connection started - Success";
}

QString extractAndRemoveUuid(QString& data)
{
    qDebug()<<"[info] Before removing uuid:"<<data;
    static QString search=QString(BLANK)+QString(BLANK);
    int pos = data.lastIndexOf(search);
    QString uuid = data.right(data.size()-pos);
    data = data.left(pos);
    qDebug()<<"[info] After removing uuid:"<<data.left(100);
    uuid.remove(0,2);
    qDebug()<<"[info] Extracted uuid:"<<uuid;
    return uuid;
}

void Controller::handleBridgeReply(QString data)
{
    QString uuid = extractAndRemoveUuid(data);
    qDebug()<<"[debug] Bridge reply with uuid:"<<uuid;
    if(responseMap.find(uuid)!=responseMap.end()){
        responseMap[uuid]->write(data.toUtf8().constData());
        qDebug()<<"[debug] Sent response to connection!";
    }
    else{
        qDebug()<<"[debug]ERROR: Doesn't match Uuid of any connections";
    }
}

void Controller::handleBridgeRequest(QString sess, TcpThread* tcpThread)
{
    qDebug()<<"[debug] Request to create instance with name:"<<sess;
    if(connectionMap.find(sess)==connectionMap.end()){
        connectionMap[sess] = tcpThread;
        QString message="Connected successfully "+sess;
        qDebug()<<"[debug]"<<message;
        //tcpThread->getSocket()->write(message.toStdString().c_str());
    }
}

QString appendUUID(QString& dataStr)
{
    QString uuid = QUuid::createUuid().toString();
    dataStr.append(BLANK);
    dataStr.append(BLANK);
    dataStr.append(uuid);
    qDebug()<<"[info] Using uuid:"<<uuid<<" for current request";
    //Use \n\nuuid
    return uuid;
}

void Controller::handleBridgeMiddle(QString sess,QString dataStr,QTcpSocket* tcpSocket)
{
    qDebug()<<"[debug] Using Proxy service for session:"<<sess;
    QString replace = QStringLiteral("/HostBoi/bridge/")+sess;
    dataStr.replace(dataStr.indexOf(replace),replace.size(),QStringLiteral(""));
    auto uuid = appendUUID(dataStr);
    responseMap[uuid] = tcpSocket;
    qDebug()<<"[info] Replaced Request:"<<dataStr.left(100);
    if(connectionMap.find(sess) != connectionMap.end()){
        qDebug()<<"[info] Sending to bridge session:"<<dataStr.toUtf8().size();
        connectionMap[sess]->getSocket()->write(dataStr.toUtf8().constData());
    }
    else{
        qDebug()<<"[info] Can't find session";
    }
}
