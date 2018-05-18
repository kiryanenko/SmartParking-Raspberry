#include "LoRaConnection.h"
#include "Worker.h"

#include <QSerialPortInfo>
#include <QDebug>
#include "SerialConnection.h"
#include "ReceiveMessageHandler.h"

Worker::Worker(QSettings *settings, QObject *parent) : QObject(parent)
{
    QList<Server*> servers;
    for(QJsonValue rec : QJsonDocument::fromJson(settings->value("servers").toByteArray()).array()) {
        servers << new Server(
                rec["host"].toString(),
                rec["mqtt_port"].toInt(),
                rec["mqtt_username"].toString(),
                rec["mqtt_password"].toString(),
                rec["login"].toString(),
                rec["password"].toString());
    }

    auto *handler = new ReceiveMessageHandler(servers, this);

    QList<quint64> sensors;
    auto sensorsInSettings = settings->value("sensors").toList();
    for (QVariant value : sensorsInSettings) {
        sensors << value.toLongLong();
    }

    QString driverType = settings->value("driver").toString().toLower();
    if (driverType == "rfm95" || driverType == "lora") {
        m_driver = new LoRaConnection(sensors,
                                      handler,
                                      settings->value("frequency").toInt(),
                                      settings->value("lora_timeout").toInt());
    } else {
        QString availablePorts = "Available ports: ";
        for (QSerialPortInfo info : QSerialPortInfo().availablePorts()) {
            availablePorts += info.portName() + ' ';
        }
        qDebug() << availablePorts;

        m_driver = new SerialConnection(sensors,
                                        handler,
                                        settings->value("serial_port").toString(),
                                        settings->value("baud_rate").toInt());
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
