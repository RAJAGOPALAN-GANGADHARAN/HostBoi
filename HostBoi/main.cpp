#include <QCoreApplication>
#include <controller.h>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    Controller appServer;
    appServer.startServer();
    return a.exec();
}
