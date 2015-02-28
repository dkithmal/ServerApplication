#include <QCoreApplication>
#include "applicationserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ApplicationServer *server= new ApplicationServer();
    server->StartServer();

    
    return a.exec();
}
