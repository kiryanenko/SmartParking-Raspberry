#include "LoRaConnection.h"
#include "Worker.h"

#include <QSerialPortInfo>
#include <QDebug>
#include "SerialConnection.h"
#include "ReceiveMessageHandler.h"

Worker::Worker(QSettings *settings, QObject *parent) : QObject(parent)
{
    QList<quint32> sensors;
    auto sensorsInSettings = settings->value("sensors").toList();
    for (QVariant value : sensorsInSettings) {
        sensors << value.toLongLong();
    }

    QList<Server*> servers;
    for(QJsonValue rec : QJsonDocument::fromJson(settings->value("servers").toByteArray()).array()) {
        servers << new Server(
                rec["host"].toString(),
                rec["mqtt_port"].toInt(),
                rec["mqtt_username"].toString(),
                rec["mqtt_password"].toString(),
                rec["login"].toString(),
                rec["password"].toString(),
                sensors,
                new AbstractReceiveMessageHandler(parent),
                parent);
    }

    QString driverType = settings->value("driver").toString().toLower();
    if (driverType == "rfm95" || driverType == "lora") {
        m_driver = new LoRaConnection(sensors,
                                      settings->value("frequency").toInt(),
                                      settings->value("lora_timeout").toInt());
    } else {
        QString availablePorts = "Available ports: ";
        for (QSerialPortInfo info : QSerialPortInfo().availablePorts()) {
            availablePorts += info.portName() + ' ';
        }
        qDebug() << availablePorts;

        m_driver = new SerialConnection(sensors,
                                        settings->value("serial_port").toString(),
                                        settings->value("baud_rate").toInt());
    }

    auto *handler = new ReceiveMessageHandler(servers, m_driver, this);
    m_driver->setHandler(handler);
    for (Server *serv : servers) {
        serv->setHandler(handler);
    }
}

Worker::~Worker()
{
    delete m_driver;
}


void Worker::run()
{
    m_driver->handleRecieveMessages();
}
