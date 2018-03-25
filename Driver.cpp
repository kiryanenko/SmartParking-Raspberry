#include "Driver.h"

#include <QDataStream>
#include <QDebug>

Driver::Driver(AbstractReceiveMessageHandler* handler) :
	m_handler(handler)
{
}

Driver::~Driver()
{
}

void Driver::handleRecieveMessages()
{
    while (available()) {
        qDebug() << "Recieve message";
        auto msg = recv();
        if (!msg.isEmpty()) {
            qDebug() << "[RECV] " << msg;
            m_handler->onRecv(msg);
            // TODO: Реализовать дальнейшую обработку
        }
    }
}

void Driver::sendSetId(uint32_t currentId, uint32_t newId)
{
    QByteArray dataToSend;
    QDataStream stream(&dataToSend, QIODevice::WriteOnly);

    stream << currentId << type_of_send_msg_set_id << newId;
    send((uint8_t*) dataToSend.data(), dataToSend.size());
}

void Driver::sendSetSensorSamplingPeriod(uint32_t sensorId, uint16_t samplingPeriod)
{
    QByteArray dataToSend;
    QDataStream stream(&dataToSend, QIODevice::WriteOnly);

    stream << sensorId << type_of_send_msg_set_sensor_sampling_period << samplingPeriod;
    send((uint8_t*) dataToSend.data(), dataToSend.size());
}

void Driver::sendSetSendingPeriod(uint32_t sensorId, uint16_t sendingPeriod)
{
    QByteArray dataToSend;
    QDataStream stream(&dataToSend, QIODevice::WriteOnly);

    stream << sensorId << type_of_send_msg_set_sending_period << sendingPeriod;
    send((uint8_t*) dataToSend.data(), dataToSend.size());
}

void Driver::sendReserve(uint32_t sensorId, uint8_t parkingPlaceId, uint16_t time)
{
    QByteArray dataToSend;
    QDataStream stream(&dataToSend, QIODevice::WriteOnly);

    stream << sensorId << type_of_send_msg_reserve << parkingPlaceId << time;
    send((uint8_t*) dataToSend.data(), dataToSend.size());
}

void Driver::sendCancelReservation(uint32_t sensorId, uint8_t parkingPlaceId)
{
    QByteArray dataToSend;
    QDataStream stream(&dataToSend, QIODevice::WriteOnly);

    stream << sensorId << type_of_send_msg_cancel_reservation << parkingPlaceId;
    send((uint8_t*) dataToSend.data(), dataToSend.size());
}
