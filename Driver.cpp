#include "Driver.h"

#include <QDebug>


Driver::Driver(QList<quint32> &sensors, AbstractReceiveMessageHandler* handler, QObject *parent) :
    m_sensors(sensors), m_handler(handler), QObject(parent)
{
}

Driver::~Driver()
{
}

void Driver::setHandler(AbstractReceiveMessageHandler *handler)
{
    m_handler = handler;
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
                    handleRecvInit(id, stream);
                } else if (type == type_of_recv_msg_payment) {
                    handleRecvPayment(id, stream);
                } else {
                    qCritical() << "[WARN] Unknown type:" << type;
                }
            } else {
                qWarning() << "[WARN] Unknown sensor ID:" << id;
            }
        }
    }
}

void Driver::sendSetId(quint32 currentId, quint32 newId)
{
    QByteArray dataToSend;
    QDataStream stream(&dataToSend, QIODevice::WriteOnly);

    stream << type_of_send_msg_set_id << currentId << newId;
    send(dataToSend);
}

void Driver::sendSetSensorSamplingPeriod(quint32 sensorId, quint16 samplingPeriod)
{
    QByteArray dataToSend;
    QDataStream stream(&dataToSend, QIODevice::WriteOnly);

    stream << type_of_send_msg_set_sensor_sampling_period << sensorId << samplingPeriod;
    send(dataToSend);
}

void Driver::sendSetSendingPeriod(quint32 sensorId, quint16 sendingPeriod)
{
    QByteArray dataToSend;
    QDataStream stream(&dataToSend, QIODevice::WriteOnly);

    stream << type_of_send_msg_set_sending_period << sensorId << sendingPeriod;
    send(dataToSend);
}

void Driver::sendSetSettings(quint32 sensorId, quint16 samplingPeriod, quint16 sendingPeriod,
                             quint16 dayCost, quint16 nightCost, quint32 dayStartTime, quint32 nightStartTime)
{
    QByteArray dataToSend;
    QDataStream stream(&dataToSend, QIODevice::ReadWrite);

    stream << type_of_send_msg_set_settings << sensorId << samplingPeriod << sendingPeriod
           << dayCost << nightCost << dayStartTime << nightStartTime;
    qDebug() << "[DEBUG] Data to send:" << dataToSend;
    send(dataToSend);
}

void Driver::sendReserve(quint32 sensorId, quint8 parkingPlaceId, quint32 time)
{
    QByteArray dataToSend;
    QDataStream stream(&dataToSend, QIODevice::ReadWrite);

    stream << type_of_send_msg_reserve << sensorId << parkingPlaceId << time;
    qDebug() << "[DEBUG] Data to send:" << dataToSend;
    qDebug() << "[DEBUG] Base method, class" << this->metaObject()->className();
    send(dataToSend);
}

void Driver::sendCancelReservation(quint32 sensorId, quint8 parkingPlaceId)
{
    QByteArray dataToSend;
    QDataStream stream(&dataToSend, QIODevice::WriteOnly);

    stream << type_of_send_msg_cancel_reservation << sensorId << parkingPlaceId;
    qDebug() << "[DEBUG] Data to send:" << dataToSend;
    send(dataToSend);
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

void Driver::handleRecvPayment(quint32 id, QDataStream &stream)
{
    quint8 place;
    quint32 time;
    quint16 payment, totalCost;
    stream >> place >> time >> payment >> totalCost;
    m_handler->onPayment(id, place, time, payment, totalCost);
}
