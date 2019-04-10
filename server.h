#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QDebug>

#include "app.h"

class Server : public QTcpServer
{
    Q_OBJECT

public:
    Server();

public slots:
    void start();

protected:
    void incomingConnection(qintptr sockDesc);

};

#endif // SERVER_H
