#include <QCoreApplication>
#include <QTimer>

#include "app.h"
#include "server.h"

int main(int argc, char *argv[])
{
    QCoreApplication core_app(argc, argv);

    App::init();

    if (!App::isConfigured)
    {
        App::log("configuration error");
        exit(EXIT_FAILURE);
    }

    Server server;
    QTimer::singleShot(1000, &server, SLOT(start()));
    return core_app.exec();
}
