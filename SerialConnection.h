#pragma once
#include "Driver.h"

#include <QSerialPort>

class SerialConnection : public Driver
{
    QSerialPort m_serial;
    const QByteArray POCKET_END = QByteArray("\r\n");

public:
    explicit SerialConnection(QList<quint64> &sensors, AbstractReceiveMessageHandler *handler, QString portName, qint32 baudRate = QSerialPort::Baud9600);
	~SerialConnection();

    bool send(const uint8_t *data, uint8_t size) override;
	bool available() override;
    QByteArray recv() override;

private:
    // Считывает все debug сообщения, возвращает true если все debug сообщения считаны.
    bool readDebugMsgs();
};
