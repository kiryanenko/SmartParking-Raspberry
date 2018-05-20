#ifndef LORACONNECTION_H
#define LORACONNECTION_H

#include "Driver.h"

#include <bcm2835.h>
#include <RH_RF95.h>
#include <RadioHead.h>

#define RF_CS_PIN  RPI_V2_GPIO_P1_24 // Slave Select on CE0 so P1 connector pin #24
#define RF_IRQ_PIN RPI_V2_GPIO_P1_22 // IRQ on GPIO25 so P1 connector pin #22
#define RF_RST_PIN RPI_V2_GPIO_P1_15 // IRQ on GPIO22 so P1 connector pin #15

class LoRaConnection : public Driver
{
    Q_OBJECT

    RH_RF95 m_rf95;

    uint16_t m_timeout;

public:
    explicit LoRaConnection(QList<quint32> &sensors, int frequency, uint16_t timeout,
                            AbstractReceiveMessageHandler *handler = new AbstractReceiveMessageHandler());
    ~LoRaConnection();

    bool send(QByteArray &data) Q_DECL_OVERRIDE;
    bool available() Q_DECL_OVERRIDE;
    QByteArray recv() Q_DECL_OVERRIDE;
};

#endif // LORACONNECTION_H
