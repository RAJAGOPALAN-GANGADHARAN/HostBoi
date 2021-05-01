#ifndef TCPTHREAD_H
#define TCPTHREAD_H

#include <QThread>
#include <QTcpSocket>
#include <QNetworkRequest>
#include <QRegularExpression>

using namespace std;

enum RequestType{
    GET,POST,
    PUT,DELETE,
    BAD
};

typedef function<void(QStringList,QTcpSocket*,RequestType)> handlerSignature;
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
public:
    TcpThread(QTcpSocket*);
    static void registerHandles(QString, handlerSignature);

public slots:
    void handleReadyRead();
    void handleCloseConnection();

private:
    void parseRequest(char* request);
    QTcpSocket* tcpSocket;
};

#endif // TCPTHREAD_H
