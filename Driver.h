#pragma once
#include <stdint.h>
#include <QByteArray>
#include <QDataStream>
#include <QList>
#include "AbstractReceiveMessageHandler.h"


class Driver
{
    const quint8 type_of_recv_msg_parking_status = 'S';
    const quint8 type_of_recv_msg_init = 'I';

    const quint8 type_of_send_msg_set_id = 'i';
    const quint8 type_of_send_msg_set_sensor_sampling_period = 's';
    const quint8 type_of_send_msg_set_sending_period = 'p';
    const quint8 type_of_send_msg_reserve = 'r';
    const quint8 type_of_send_msg_cancel_reservation = 'c';

    AbstractReceiveMessageHandler *m_handler;

    QList<quint64> m_sensors;

public:
    explicit Driver(QList<quint64> &sensors, AbstractReceiveMessageHandler *handler);
	virtual ~Driver();

    virtual bool send(const uint8_t *data, uint8_t size) = 0;
	virtual bool available() = 0;
    virtual QByteArray recv() = 0;

	void handleRecieveMessages();

    void sendSetId(uint32_t sensorId, uint32_t newId);
    void sendSetSensorSamplingPeriod(uint32_t sensorId, uint16_t samplingPeriod);
    void sendSetSendingPeriod(uint32_t sensorId, uint16_t sendingPeriod);
    void sendReserve(uint32_t sensorId, uint8_t parkingPlaceId, uint16_t time);
    void sendCancelReservation(uint32_t sensorId, uint8_t parkingPlaceId);

private:
    void handleRecvParkingState(quint32 id, QDataStream &stream);
    void handleRecvInit(quint32 id, QDataStream &stream);
};
