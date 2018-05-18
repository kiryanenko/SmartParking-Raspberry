#ifndef LORACONNECTION_H
#define LORACONNECTION_H

#include "Driver.h"

#include <RadioHead.h>

class LoRaConnection : public Driver
{
public:
    explicit LoRaConnection(QList<quint64> &sensors, AbstractReceiveMessageHandler *handler, int frequency);
    ~LoRaConnection();

    bool send(const uint8_t *data, uint8_t size) override;
    bool available() override;
    QByteArray recv() override;
};

#endif // LORACONNECTION_H
