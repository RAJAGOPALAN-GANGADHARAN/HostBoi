#ifndef HANDLERS_H
#define HANDLERS_H

#include <QStringList>
#include <fstream>
#include "tcpthread.h"
#include "enums.h"

#define handlerParam [](QString headerInfo,QString dataStr,QTcpSocket* socket, RequestType type)->pair<QString,ProcessType>

using namespace std;

string readHtmlFile(string fileName)
{
    fstream f(fileName,ios::in);
    string lineReader;
    string html="";
    while(getline(f,lineReader)){
        html.append(lineReader);
        html.append(BLANK);
    }
    f.close();
    return html;
}
void setOkReponse(string& message){
    message.append("HTTP/1.1 200 OK");
    message.append(CRLF);
}
void setPlainType(string& message){
    message.append("Content-Type: text/plain");
    message.append(CRLF);
}
void setHtmlType(string& message){
    message.append("Content-Type: text/html;charset=UTF-8");
    message.append(CRLF);
}
void setContentLength(string& message,int size){
    message.append("Content-Length: ");
    message.append(to_string(size));
}
void addBlanks(string& message){
    message.append(BLANK);
    message.append(BLANK);
}

string quickTextResponse(string text){
    string message="";
    setOkReponse(message);
    setPlainType(message);
    setContentLength(message,text.size());
    addBlanks(message);
    message.append(text);
    return message;
}

string quickHtmlResponse(string fileName){
    string html = readHtmlFile(fileName);
    string message = "";

    setOkReponse(message);
    setHtmlType(message);
    setContentLength(message,html.size());
    addBlanks(message);

    message.append(html);
    return message;
}
auto homeRequest = handlerParam{
    string message = quickHtmlResponse("static/home.html");
    socket->write(message.c_str());
    return {"",NO_ACTION};
};

auto badRequest = handlerParam{
    string badResponse = quickTextResponse("Bad Request!");
    socket->write(badResponse.c_str());
    return {"",NO_ACTION};
};

auto createRequest = handlerParam {
    if(type!=GET){
        qDebug()<<"[debug] Can't use other request type for create!";
        return {"",NO_ACTION};
    }
    qDebug()<<"[debug] create proxy instance";
    return {"",NO_ACTION};
};

auto bridgeConnectService = handlerParam {
    auto reqHead = headerInfo.split(" ");
    auto uri = reqHead[1].split("/",Qt::SkipEmptyParts);
    if(uri.size()!=3){
        socket->write(quickTextResponse("Please use /HostBoi/create/<custom_sess> to create"\
        "a bridge session").c_str());
        return {"",NO_ACTION};
    }
    QString sess = uri.back();
    return {sess,CREATE_ACTION};
};

auto bridgeService = handlerParam {
    auto reqHead = headerInfo.split(" ");
    auto uri = reqHead[1].split("/",Qt::SkipEmptyParts);
    if(uri.size()>=3 && uri[0]=="HostBoi" && uri[1]=="bridge"){
        QString sess = uri[2];
        return {sess,BRIDGE_ACTION};
    }
    return {"",NO_ACTION};
};

#endif // HANDLERS_H
