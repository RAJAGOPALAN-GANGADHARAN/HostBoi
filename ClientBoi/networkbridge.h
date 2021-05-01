#ifndef NETWORKBRIDGE_H
#define NETWORKBRIDGE_H

#include <QObject>

class NetworkBridge : public QObject
{
    Q_OBJECT
public:
    explicit NetworkBridge(QObject *parent = nullptr);

signals:

};

#endif // NETWORKBRIDGE_H
