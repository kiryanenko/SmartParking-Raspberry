#include "SerialConnection.h"
#include <QDataStream>

#include <QDebug>

SerialConnection::SerialConnection(QString portName, AbstractReceiveMessageHandler *handler) :
	Driver(handler)
{
    qInfo() << "Connect to serial port: " << portName;
    m_serial.setPortName(portName);
    m_serial.setBaudRate(QSerialPort::Baud9600);
    m_serial.open(QIODevice::ReadWrite);
}


SerialConnection::~SerialConnection()
{
    m_serial.close();
}


bool SerialConnection::send(const uint8_t* data, const uint8_t size)
{
    QByteArray dataToSend((char*) &size, sizeof(size));
    dataToSend.append((char*) data, size);

qDebug() << dataToSend;

    auto sendedCount = m_serial.write(dataToSend);
    return sendedCount == size;
}


bool SerialConnection::available()
{
    return m_serial.bytesAvailable() > 0;
}


uint8_t* SerialConnection::recv(size_t& size)
{
    uint8_t len;
    size = m_serial.read((char *) &len, sizeof(len));

    if (size < sizeof(len) || len == '[') {
        qDebug() << '[' + m_serial.readLine();
        return nullptr;
    }

    const auto buf = new uint8_t[len];
    size = m_serial.read((char *) buf, len);

    if (size != len) {
        delete[] buf;
        return nullptr;
    }

    return buf;
}
