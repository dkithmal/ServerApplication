#ifndef APPLICATIONSERVER_H
#define APPLICATIONSERVER_H

#include <QTcpServer>
#include <QDebug>
#include <QString>
#include"clientcommandhandler.h"


class ApplicationServer : public QTcpServer
{
public:
    ApplicationServer();
    void StartServer();

protected:
    void incomingConnection(int socketDescriptor);
};

#endif // APPLICATIONSERVER_H
