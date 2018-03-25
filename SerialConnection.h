#pragma once
#include "Driver.h"

#include <QSerialPort>

class SerialConnection : public Driver
{
    QSerialPort m_serial;
    const QByteArray POCKET_END = QByteArray("\r\n");

public:
    explicit SerialConnection(QString portName, AbstractReceiveMessageHandler *handler);
	~SerialConnection();

    bool send(const uint8_t *data, uint8_t size) override;
	bool available() override;
    QByteArray recv() override;
};
