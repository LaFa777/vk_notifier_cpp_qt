#include "vknotifer.h"

#include <QApplication>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    app.setOrganizationName("VK");
    app.setApplicationName("Vk Notifer");

    VkNotifer vk;
    vk.start();

    // vk.checkMessage( "account.getCounters" );

    return app.exec();
}
