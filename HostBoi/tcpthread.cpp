#include "tcpthread.h"

/*
 * connect to bridge client through /HostBoi/connect/<sess>/
 * bridge request via url /HostBoi/bridge/<sess>/<...>
 * * generate uuid and add conection socket and uuid mapping
 * * send request to client along with uuid
 * * last line \n\nuuid pass around factor
 * * reply back with uuid
 * bridge connects to target
 *
 *
 *
 */
handlerMap HandlerMap::handlerMap_ = {
    {"",nullptr}
};

patternList HandlerMap::patternList_ = {
    QRegularExpression("")
};

pair<QString,QString> getPattern(QString header)
{
    auto splitList = header.split(" ");
    if(splitList.size()!=3){
        qDebug()<<"[debug] Malformed Request";
        return {"",""};
    }

    qDebug()<<"[info]Looking for pattern "<<splitList[1];

    //Test regex on all patterns and return the pattern
    for(auto pattern: HandlerMap::getAllPatterns()){
        auto match = pattern.match(splitList[1]);
        if(match.hasMatch() && pattern.pattern()!=""){
            qDebug()<<"[info]"<<pattern.pattern();
            return {pattern.pattern(),splitList[1]};
        }
    }

    return {"/404","/404"};
}

void TcpThread::parseRequest(QByteArray dataStr)
{
    int index = dataStr.indexOf(CRLF);
    if(index==-1)
    {
        qDebug()<<"[debug] Empty Request or doesn't have \r\n in the request.";
        return;
    }

    QString requestInfo = dataStr.left(index);
    qDebug()<<requestInfo<<"-"<<index;
    RequestType type;
    if(requestInfo.startsWith(QStringLiteral("GET")))
    {
        qDebug()<<"[info] Get Request";
        type = GET;
    }
    else if(requestInfo.startsWith(QStringLiteral("PUT")))
    {
        qDebug()<<"[info] Put Request";
        type = PUT;
    }
    else if(requestInfo.startsWith(QStringLiteral("POST")))
    {
        qDebug()<<"[info] Post Request";
        type = POST;
    }
    else if(requestInfo.startsWith(QStringLiteral("DELETE")))
    {
        qDebug()<<"[info] Delete Request";
        type = DELETE;
    }
    else if(requestInfo.startsWith(QStringLiteral("HTTP")))
    {
        qDebug()<<"[info] Http Response";
        type = RESPONSE;
        emit bridgeReply(dataStr);
        return;
    }
    else
    {
        qDebug()<<"[info] Http Response";
        type = RESPONSE;
        emit bridgeReply(dataStr);
        //qDebug()<<"[info] No specific form found for the Request";
        return;
    }

    pair<QString,QString> urlPattern = getPattern(requestInfo);
    if(HandlerMap::isPresent(urlPattern.first)){
        auto func = HandlerMap::getHandler(urlPattern.first);
        auto resp = func(requestInfo,dataStr,tcpSocket,type);
        switch(resp.second)
        {
        case CREATE_ACTION:
            emit isBridgeRequest(resp.first,this);
            //handleBridgeCreate(resp.first);
            break;
        case BRIDGE_ACTION:
            emit bridgeRequest(resp.first,dataStr,tcpSocket);
            //handleBridge(resp.first,dataStr);
            break;
        default:
            qDebug()<<"[debug] No action to do";
        }
    }
}

TcpThread::TcpThread(QTcpSocket* socket)
{
    tcpSocket = socket;
    connect(tcpSocket,&QTcpSocket::readyRead,
            this,&TcpThread::handleReadyRead);
    connect(tcpSocket,&QTcpSocket::disconnected,
            this,&TcpThread::handleCloseConnection);
    connect(tcpSocket, &QTcpSocket::bytesWritten,
            this, &TcpThread::bytesReceived);
}

void TcpThread::registerHandles(QString str, handlerSignature f)
{
    HandlerMap::patternList_.push_back(QRegularExpression(str));
    HandlerMap::handlerMap_[str] = f;
}

void TcpThread::bytesReceived(qint64 bytes){
    qDebug()<<"[debug] Recieved "<<bytes<<" from bridge";
}

void TcpThread::handleReadyRead()
{
    qDebug()<<"[debug] Parsing request from client";
    QByteArray requestData = tcpSocket->readAll();
    qDebug()<<"[info] Recieved Data:";
    qDebug()<<requestData.left(100);
    parseRequest(requestData);
}


void TcpThread::handleCloseConnection()
{
    qDebug()<<"[debug] Client Disconnecting";
    tcpSocket->close();
    qDebug()<<"[debug] Client Disconnected";
    this->quit();
    qDebug()<<"[debug] Kill Tcp thread";
}
