#include "LoRaConnection.h"

LoRaConnection::LoRaConnection(QList<quint64> &sensors, AbstractReceiveMessageHandler *handler, int frequency) :
    Driver(sensors, handler)
{

}

LoRaConnection::~LoRaConnection()
{
}

bool LoRaConnection::send(const uint8_t *data, uint8_t size)
{
    return false;
}

bool LoRaConnection::available()
{
    return false;
}

QByteArray LoRaConnection::recv()
{
    return QByteArray();
}
