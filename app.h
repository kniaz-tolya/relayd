#ifndef APP_H
#define APP_H

#include <QObject>
#include <QString>
#include <QSettings>
#include <QByteArray>
#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QFile>
#include <QIODevice>
#include <QStringList>
#include <QProcessEnvironment>
#include <QTextStream>
#include <QDateTime>

#include <QDebug>

class App
{
public:
    static void init(void);

    static QString encrypt(QString str);
    static QString decrypt(QString str);


    static void log_with_time(QString str);
    static void log(QString str);
    static void hr(void);

private:
    static void dump_config(void);

public:
    static bool isConfigured;
    static int listenPort;
    static QString forwardAddr;
    static int forwardPort;
    static int xferMode;
    static uint64_t key;

private:
    static QTextStream ts;
};

#endif // APP_H
