#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QNetworkRequest>
#include <QRegularExpression>
#include "enums.h"

using namespace std;

typedef function<pair<QString,ProcessType>(QString,QString,
                                           QTcpSocket*,RequestType)> handlerSignature;
typedef map<QString, handlerSignature> handlerMap;
typedef vector<QRegularExpression> patternList;

class HandlerMap
{
public:
    static patternList getAllPatterns(){
        return patternList_;
    }
    static bool isPresent(QString pattern){
        return handlerMap_.find(pattern)!=handlerMap_.end();
    }
    static handlerSignature getHandler(QString pattern){
        return handlerMap_[pattern];
    }

private:
    static handlerMap handlerMap_;
    static patternList patternList_;
    friend class TcpThread;
};

class TcpThread : public QThread
{
    Q_OBJECT
public:
    TcpThread(QTcpSocket*);
    QTcpSocket* getSocket(){
        //Should be removed
        return tcpSocket;
    }
    static void registerHandles(QString, handlerSignature);
signals:
    void isBridgeRequest(QString,TcpThread*);
    void bridgeRequest(QString,QString,QTcpSocket*);
    void bridgeReply(QString);

public slots:
    void handleReadyRead();
    void handleCloseConnection();

private:
    void parseRequest(char* request);
    QTcpSocket* tcpSocket;
};

#endif // TCPTHREAD_H
