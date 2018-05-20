#include "SerialConnection.h"
#include <QDataStream>

#include <QDebug>

SerialConnection::SerialConnection(QList<quint32> &sensors, QString portName, qint32 baudRate,
                                   AbstractReceiveMessageHandler *handler, QObject *parent) :
    Driver(sensors, handler, parent)
{
    qInfo() << "Connect to serial port: " << portName;
    m_serial.setPortName(portName);
    m_serial.setBaudRate(baudRate);
    m_serial.open(QIODevice::ReadWrite);
}


SerialConnection::~SerialConnection()
{
    m_serial.close();
}


bool SerialConnection::send(QByteArray data)
{
    char len = data.size();
    data.prepend(len);
    qDebug() << "[SEND] " << data;
    auto sendedCount = m_serial.write(data);
    return sendedCount == data.size();
}


bool SerialConnection::available()
{
    if (!m_serial.isOpen()) {
        qFatal("Serial port is closed.");
        exit(1);
        return false;
    }

    if (!readDebugMsgs()) {
        return false;
    }

    auto available = m_serial.bytesAvailable();
    if (available == 0) {
        return false;
    }

    uint8_t len;
    m_serial.peek((char *) &len, sizeof(len));

    // Формат пакета: |size(data)|data|\r\n|
    auto pocketSize = len + sizeof(len) + POCKET_END.size();
    if (available >= pocketSize) {
        auto buf = m_serial.peek(pocketSize);
        if (!buf.endsWith(POCKET_END)) {
            qCritical() << "Bad serial pocket: " << buf;
            m_serial.clear();
            return false;
        }
    }

    return true;
}


QByteArray SerialConnection::recv()
{
    uint8_t len;
    m_serial.read((char *) &len, sizeof(len));
    auto data = m_serial.read(len);
    m_serial.read(POCKET_END.size());
    return data;
}

bool SerialConnection::readDebugMsgs()
{
    uint8_t len;
    auto size = m_serial.peek((char *) &len, sizeof(len));
    while (size == sizeof(len) && len == '[') {
        if (!m_serial.canReadLine()) {
            return false;
        }
        qDebug() << "[RECV DEBUG]" << m_serial.readLine();
        size = m_serial.peek((char *) &len, sizeof(len));
    }
    return true;
}
