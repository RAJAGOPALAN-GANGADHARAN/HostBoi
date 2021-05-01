#include "controller.h"
#include "handlers.h"

Controller::Controller(QObject *parent) : QObject(parent)
{
    httpServer = new QTcpServer(this);
    TcpThread::registerHandles("^/home$",homeRequest);
    TcpThread::registerHandles("/404",badRequest);
    TcpThread::registerHandles("^/HostBoi/create$",createRequest);
}

void Controller::startServer()
{
    //Connect slots
    connect(httpServer,&QTcpServer::newConnection,
            this,&Controller::handleNewConnection);

    httpServer->listen(QHostAddress::Any, 80);
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
    connectionThread->start();

    qDebug()<<"[debug] Incoming connection started - Success";
}

