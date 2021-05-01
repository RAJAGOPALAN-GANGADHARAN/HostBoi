#include "tcpthread.h"

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

void TcpThread::parseRequest(char* request)
{
    QString dataStr(request);
    auto splitList = dataStr.split("\r\n");
    if(splitList.size()<=0)
    {
        qDebug()<<"[debug] Empty Request";
        return;
    }
    RequestType type;
    if(splitList[0].startsWith(QStringLiteral("GET")))
    {
        qDebug()<<"[info] Get Request";
        type = GET;
    }
    else if(splitList[0].startsWith(QStringLiteral("PUT")))
    {
        qDebug()<<"[info] Put Request";
        type = PUT;
    }
    else if(splitList[0].startsWith(QStringLiteral("POST")))
    {
        qDebug()<<"[info] Post Request";
        type = POST;
    }
    else if(splitList[0].startsWith(QStringLiteral("DELETE")))
    {
        qDebug()<<"[info] Delete Request";
        type = DELETE;
    }
    else
    {
        qDebug()<<"[info] Bad Request";
        type = BAD;
        return;
    }

    pair<QString,QString> urlPattern = getPattern(splitList[0]);
    if(HandlerMap::isPresent(urlPattern.first)){
        auto func = HandlerMap::getHandler(urlPattern.first);
        func(splitList,tcpSocket,type);
    }
}

TcpThread::TcpThread(QTcpSocket* socket)
{
    tcpSocket = socket;
    connect(tcpSocket,&QTcpSocket::readyRead,
            this,&TcpThread::handleReadyRead);
    connect(tcpSocket,&QTcpSocket::disconnected,
            this,&TcpThread::handleCloseConnection);
}

void TcpThread::registerHandles(QString str, handlerSignature f)
{
    HandlerMap::patternList_.push_back(QRegularExpression(str));
    HandlerMap::handlerMap_[str] = f;
}

void TcpThread::handleReadyRead()
{
    qDebug()<<"[debug] Parsing request from client";
    QByteArray requestData = tcpSocket->readAll();
    parseRequest(requestData.data());
}


void TcpThread::handleCloseConnection()
{
    qDebug()<<"[debug] Client Disconnecting";
    tcpSocket->close();
    qDebug()<<"[debug] Client Disconnected";
    this->quit();
    qDebug()<<"[debug] Kill Tcp thread";
}
