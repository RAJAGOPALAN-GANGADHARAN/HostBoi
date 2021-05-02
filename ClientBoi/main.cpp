#include <QCoreApplication>
#include <QTcpSocket>
#include <networkbridge.h>
int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    NetworkBridge bridge;
    bridge.connectToServer("localhost");
    return a.exec();
}
