#include <QCoreApplication>
#include <QSettings>
#include <QTimer>
#include <QDebug>
#include <QSerialPortInfo>
#include <QJsonArray>
#include "Worker.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QList<QVariant> defaultSensors;
    defaultSensors << 1 << 2;


    QJsonObject server1 = {
        {"host", "0.0.0.0"},
        {"mqtt_port", 1883},
        {"mqtt_username", ""},
        {"mqtt_password", ""},
        {"login", "user1@mail.ru"},
        {"password", "pwd123456"}
    };
    QJsonObject server2 = {
        {"host", "m23.cloudmqtt.com"},
        {"mqtt_port", 16325},
        {"mqtt_username", "mwkexjvt"},
        {"mqtt_password", "Wnx76uUjIVK4"},
        {"login", "user1@mail.ru"},
        {"password", "pwd123456"}
    };
    QJsonArray servers = { server1, server2 };



    QSettings *settings = new QSettings("settings.ini", QSettings::IniFormat, &a);
    if (settings->value("sensors").isNull()) settings->setValue("sensors", QVariant(defaultSensors));
    if (settings->value("servers").isNull()) settings->setValue("servers", QString(QJsonDocument(servers).toJson()).replace(' ', ""));
    if (settings->value("driver").isNull()) settings->setValue("driver", "serial");
    if (settings->value("serial_port").isNull()) settings->setValue("serial_port", QSerialPortInfo().availablePorts()[0].portName());
    if (settings->value("baud_rate").isNull()) settings->setValue("baud_rate", 9600);
    if (settings->value("frequency").isNull()) settings->setValue("frequency", 868);
    if (settings->value("lora_timeout").isNull()) settings->setValue("lora_timeout", 1000);
    if (settings->value("idle_period").isNull()) settings->setValue("idle_period", 300);
    settings->sync(); //записываем настройки

    Worker *worker = new Worker(settings, &a);
    QTimer *timer = new QTimer(&a);
    QObject::connect(timer, SIGNAL(timeout()), worker, SLOT(run()));
    timer->start(settings->value("idle_period").toInt());

    return a.exec();
}
