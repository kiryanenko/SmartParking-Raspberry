#pragma once

#include <QtCore>
#include <QByteArray>
#include <QDataStream>
#include <QList>
#include "AbstractReceiveMessageHandler.h"


class Driver : public QObject
{
    Q_OBJECT


    const quint8 type_of_recv_msg_parking_status = 'S';
    const quint8 type_of_recv_msg_init = 'I';
    const quint8 type_of_recv_msg_payment = 'P';

    const quint8 type_of_send_msg_set_id = 'i';
    const quint8 type_of_send_msg_set_sensor_sampling_period = 's';
    const quint8 type_of_send_msg_set_sending_period = 'p';
    const quint8 type_of_send_msg_reserve = 'r';
    const quint8 type_of_send_msg_cancel_reservation = 'c';


    AbstractReceiveMessageHandler *m_handler;

    QList<quint32> m_sensors;

public:
    explicit Driver(QList<quint32> &sensors, AbstractReceiveMessageHandler *handler = new AbstractReceiveMessageHandler(),
                    QObject *parent = 0);
    virtual ~Driver();

    virtual bool send(QByteArray data) = 0;
    virtual bool available() = 0;
    virtual QByteArray recv() = 0;

    void setHandler(AbstractReceiveMessageHandler *handler);

	void handleRecieveMessages();

    void sendSetId(quint32 sensorId, quint32 newId);
    void sendSetSensorSamplingPeriod(quint32 sensorId, quint16 samplingPeriod);
    void sendSetSendingPeriod(quint32 sensorId, quint16 sendingPeriod);
    void sendReserve(quint32 sensorId, quint8 parkingPlaceId, quint32 time);
    void sendCancelReservation(quint32 sensorId, quint8 parkingPlaceId);

private:
    void handleRecvParkingState(quint32 id, QDataStream &stream);
    void handleRecvInit(quint32 id, QDataStream &stream);
    void handleRecvPayment(quint32 id, QDataStream &stream);
};
