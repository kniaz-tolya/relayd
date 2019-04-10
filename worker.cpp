#include "worker.h"

Worker::Worker(QObject *parent) : QObject(parent)
{
    this->pSockNext_ = nullptr;
    this->pSockPrev_ = nullptr;
}

void Worker::start(qintptr sockDesc)
{
    this->sockServer_ = sockDesc;
    App::log(QString("[%1] worker started.").arg(this->sockServer_));
    pSockPrev_ = new QTcpSocket;
    if (!pSockPrev_->setSocketDescriptor(sockServer_))
    {
        App::log(pSockPrev_->errorString());
        return;
    }
    QObject::connect(pSockPrev_, SIGNAL(readyRead()), this, SLOT(forward()));
    QObject::connect(pSockPrev_, SIGNAL(disconnected()), this, SLOT(prevStageDisconnected_()));

    pSockNext_ = new QTcpSocket;
    pSockNext_->connectToHost(App::forwardAddr, App::forwardPort);
    if (!pSockNext_->waitForConnected(2000))
    {
        App::log(pSockNext_->errorString());
        return;
    }
    QObject::connect(pSockNext_, SIGNAL(readyRead()), this, SLOT(backward()));
    QObject::connect(pSockNext_, SIGNAL(disconnected()), this, SLOT(nextStageDisconnected_()));
    App::log(QString("[%1] next_stage connected.").arg(this->sockServer_));
}

void Worker::forward()
{
    while (this->pSockPrev_->canReadLine())
    {
        QByteArray ba = this->pSockPrev_->readLine();
        QString input = QString::fromUtf8(ba).trimmed();
        QString output;
        switch (App::xferMode)
        {
        case 00: // input plain, output plain
            output = input;
            break;
        case 01: // input plain, output cipher
            output = App::encrypt(input);
            break;
        case 10: // input cipher, output plain
            output = App::decrypt(input);
            break;
        case 11: // input cipher, output cipher
            output = input;
            break;
        default:
            App::log("invalid xferMode");
            exit(EXIT_FAILURE);
        }
        App::log(QString("Forward:\t%1").arg(output));
        output.append('\n');
        pSockNext_->write(output.toUtf8());
    }
}

void Worker::backward()
{
    while (pSockNext_->canReadLine())
    {
        QByteArray ba = this->pSockNext_->readLine();
        QString input = QString::fromUtf8(ba).trimmed();
        QString output;
        switch (App::xferMode)
        {
        case 00: // input plain, output plain
            output = input;
            break;
        case 01: // input cipher, output plain
            output = App::decrypt(input);
            break;
        case 10: // input plain, output cipher
            output = App::encrypt(input);
            break;
        case 11: // input cipher, output cipher
            output = input;
            break;
        default:
            App::log("invalid xferMode");
            exit(EXIT_FAILURE);
        }
        output.append('\n');
        App::log(QString("Backward:\t%1").arg(output));
        pSockPrev_->write(output.toUtf8());
    }
}

void Worker::nextStageDisconnected_()
{
    App::log(QString("[%1] next_stage_disconnected").arg(this->sockServer_));
    pSockNext_->deleteLater();
    pSockNext_ = nullptr;
    if (pSockPrev_)
    {
        pSockPrev_->disconnectFromHost();
    }
    else
    {
        this->deleteLater();
    }
}

void Worker::prevStageDisconnected_()
{
    App::log(QString("[%1] prev_stage_disconnected").arg(this->sockServer_));
    pSockPrev_->deleteLater();
    pSockPrev_ = nullptr;
    if (pSockNext_)
    {
        pSockNext_->disconnectFromHost();
    }
    else
    {
        this->deleteLater();
    }
}

