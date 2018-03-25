#include "Worker.h"

#include <QSerialPortInfo>
#include <QDebug>
#include "SerialConnection.h"
#include "ReceiveMessageHandler.h"

Worker::Worker(QSettings *settings, QObject *parent) : QObject(parent)
{
    auto *handler = new ReceiveMessageHandler();

    QString driverType = settings->value("driver").toString().toLower();
    if (driverType == "rfm95") {
    } else {
        QString availablePorts = "Available ports: ";
        for (QSerialPortInfo info : QSerialPortInfo().availablePorts()) {
            availablePorts += info.portName() + ' ';
        }
        qDebug() << availablePorts;

        m_driver = new SerialConnection(handler, settings->value("serial_port").toString(), settings->value("baud_rate").toInt());
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
