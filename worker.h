#ifndef WORKER_H
#define WORKER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QTcpSocket>
#include <QString>
#include <QByteArray>
#include <QFile>

#include "app.h"

class Worker : public QObject
{
    Q_OBJECT

public:
    Worker(QObject *parent);
    void start(qintptr sockDesc);

private slots:
    void forward();
    void backward();
    void prevStageDisconnected_();
    void nextStageDisconnected_();

private:
    qintptr sockServer_;
    QTcpSocket *pSockPrev_;
    QTcpSocket *pSockNext_;
};

#endif // WORKER_H
