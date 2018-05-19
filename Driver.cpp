#include "Driver.h"

#include <QDebug>

Driver::Driver(QList<quint64> &sensors, AbstractReceiveMessageHandler* handler) :
    m_sensors(sensors), m_handler(handler)
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
            m_handler->onRecv(msg);
            QDataStream stream(&msg, QIODevice::ReadOnly);
            quint8 type;
            quint32 id;
            stream >> type >> id;
            qDebug() << "[DEBUG] Recv msg type:" << type << "; sensor ID:" << id;

            if (m_sensors.indexOf(id) != -1) {
                if (type == type_of_recv_msg_parking_status) {
                    handleRecvParkingState(id, stream);
                } else if (type == type_of_recv_msg_init) {
                    qDebug() << "dddddgggggggggggggggggggggdddddd";
                    handleRecvInit(id, stream);
                } else {
                    qCritical() << "[WARN] Unknown type:" << type;
                }
            } else {
                qWarning() << "[WARN] Unknown sensor ID:" << id;
            }
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

void Driver::handleRecvParkingState(quint32 id, QDataStream &stream)
{
    quint8 place;
    bool isFree;
    stream >> place >> isFree;
    m_handler->onParkingStatus(id, place, isFree);
}

void Driver::handleRecvInit(quint32 id, QDataStream &stream)
{
    quint16 samplingPeriod, sendingPeriod;
    quint16 dayCost, nightCost;
    quint32 dayStartTime, nightStartTime;
    stream >> samplingPeriod >> sendingPeriod >> dayCost >> nightCost >> dayStartTime >> nightStartTime;
    m_handler->onInit(id, samplingPeriod, sendingPeriod, dayCost, nightCost, dayStartTime, nightStartTime);
}
