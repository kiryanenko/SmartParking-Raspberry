#pragma once
#include <stdint.h>
#include <QByteArray>
#include "AbstractReceiveMessageHandler.h"


class Driver
{
    const uint8_t type_of_recv_msg_parking_status = 'S';
    const uint8_t type_of_recv_msg_init_status = 'I';

    const uint8_t type_of_send_msg_set_id = 'i';
    const uint8_t type_of_send_msg_set_sensor_sampling_period = 's';
    const uint8_t type_of_send_msg_set_sending_period = 'p';
    const uint8_t type_of_send_msg_reserve = 'r';
    const uint8_t type_of_send_msg_cancel_reservation = 'c';

    AbstractReceiveMessageHandler *m_handler;

public:
    explicit Driver(AbstractReceiveMessageHandler *handler);
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
};
