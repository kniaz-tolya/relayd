#include "server.h"
#include "worker.h"

Server::Server()
{
    App::hr();
}

void Server::start()
{
    if (!this->listen(QHostAddress::Any, App::listenPort))
        App::log("listen error.");
    else
        App::log("server is listening.");
}

void Server::incomingConnection(qintptr sockDesc)
{
    App::log("incoming connection.");
    Worker * pWorker = new Worker(this);
    pWorker->start(sockDesc);
}
