#include "LoRaConnection.h"

LoRaConnection::LoRaConnection(QList<quint32> &sensors, int frequency, uint16_t timeout,
                               AbstractReceiveMessageHandler *handler) :
    m_rf95(RF_CS_PIN, RF_IRQ_PIN), m_timeout(timeout), Driver(sensors, handler)
{
    if (!bcm2835_init()) {
      qCritical() << "bcm2835_init() Failed";
    }

    pinMode(RF_IRQ_PIN, INPUT);
    bcm2835_gpio_set_pud(RF_IRQ_PIN, BCM2835_GPIO_PUD_DOWN);
    // Now we can enable Rising edge detection
    bcm2835_gpio_ren(RF_IRQ_PIN);

    pinMode(RF_RST_PIN, OUTPUT);
    digitalWrite(RF_RST_PIN, LOW );
    bcm2835_delay(150);
    digitalWrite(RF_RST_PIN, HIGH );
    bcm2835_delay(100);

    m_rf95.init();
    m_rf95.setFrequency(frequency);
    m_rf95.setTxPower(14, false);
    m_rf95.setPromiscuous(true);
}

LoRaConnection::~LoRaConnection()
{
}

bool LoRaConnection::send(QByteArray data)
{
    qDebug() << "[SEND] Msg[" << data.size() << "]:" << data;
    m_rf95.send((uint8_t*) data.data(), data.size());
    const auto isSend = m_rf95.waitPacketSent();

    if (!isSend) {
        qCritical() << "[ERROR] RFM95 do not send data:" << data;
    }
    return isSend;
}

bool LoRaConnection::available()
{
    return m_rf95.available();
}

QByteArray LoRaConnection::recv()
{
    uint8_t *buf = new uint8_t[RH_RF95_MAX_MESSAGE_LEN];
    uint8_t len = RH_RF95_MAX_MESSAGE_LEN;
    m_rf95.recv(buf, &len);
    return QByteArray((char *) buf, len);
}
