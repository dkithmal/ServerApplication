#include "applicationserver.h"

ApplicationServer::ApplicationServer()
{

}


void ApplicationServer::StartServer()
{
    if(!this->listen(QHostAddress::Any,1111))
    {
        qDebug()<<"could not start server";
    }
    else
    {
        qDebug()<<"listening...";
    }


}


void ApplicationServer::incomingConnection(int socketDescriptor)
{
    qDebug()<<socketDescriptor<<"connecting....";

    ClientCommandHandler *clientCommandHandler = new ClientCommandHandler(socketDescriptor,this);
    connect(clientCommandHandler,SIGNAL(finished()),clientCommandHandler,SLOT(deleteLater()));

    clientCommandHandler->start();
}

