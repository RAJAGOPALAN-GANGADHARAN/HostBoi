#ifndef HANDLERS_H
#define HANDLERS_H

#include <QStringList>
#include <fstream>
#include "tcpthread.h"

#define CRLF "\r\n"
#define BLANK "\n"
#define handlerParam QStringList splitList,QTcpSocket* socket, RequestType type

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

auto homeRequest = [](handlerParam){
    string html = readHtmlFile("static/home.html");
    string message = "";
    message.append("HTTP/1.1 200 OK");
    message.append(CRLF);
    message.append("Content-Type: text/html;charset=UTF-8");
    message.append(CRLF);
    message.append("Content-Length: ");
    message.append(to_string(html.size()));
    message.append(BLANK);
    message.append(BLANK);
    message.append(html);

    socket->write(message.c_str());
};

auto badRequest = [](handlerParam){
    char *bad = "HTTP/1.1 404 OK\nContent-Type: text/plain\nContent-Length: 11\n\nBad Request";
    socket->write(bad);
};

auto createRequest = [](handlerParam) {
    if(type!=GET){
        qDebug()<<"[debug] Can't use other request type for create!";
        return;
    }
    qDebug()<<"[debug] create proxy instance";

};

#endif // HANDLERS_H
