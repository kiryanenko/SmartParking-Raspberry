#pragma once
#include "Driver.h"

#include <QSerialPort>

class SerialConnection : public Driver
{
    Q_OBJECT

    QSerialPort m_serial;
    const QByteArray POCKET_END = QByteArray("\r\n");

public:
    explicit SerialConnection(QList<quint32> &sensors, QString portName, qint32 baudRate = QSerialPort::Baud9600,
                              AbstractReceiveMessageHandler *handler = new AbstractReceiveMessageHandler(), QObject *parent = 0);
	~SerialConnection();

    bool send(QByteArray data) Q_DECL_OVERRIDE;
    bool available() Q_DECL_OVERRIDE;
    QByteArray recv() Q_DECL_OVERRIDE;

private:
    // Считывает все debug сообщения, возвращает true если все debug сообщения считаны.
    bool readDebugMsgs();
};
