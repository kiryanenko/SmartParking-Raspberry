#pragma once

#include "AbstractReceiveMessageHandler.h"
#include "Server.h"
#include "Driver.h"

class ReceiveMessageHandler : public AbstractReceiveMessageHandler
{
    QList<Server*> m_servers;
    Driver *m_driver;

public:
    ReceiveMessageHandler(QList<Server*> &servers, Driver *driver, QObject *parent = 0);
    ~ReceiveMessageHandler();

    void onRecv(QByteArray data) override;
    void onParkingStatus(quint32 id, quint8 place, bool isFree) override;
    void onInit(quint32 id,
                quint16 samplingPeriod,
                quint16 sendingPeriod,
                quint16 dayCost,
                quint16 nightCost,
                quint32 dayStartTime,
                quint32 nightStartTime) override;
    void onPayment(quint32 id, quint8 place, quint32 bookedTime, quint16 payment, quint16 totalCost) override;

    void onBook(quint32 sensor, quint8 place, quint32 bookedTime) override;
    void onSetSensorSettings(quint32 sensor,
                             quint16 samplingPeriod,
                             quint16 sendingPeriod,
                             quint16 dayCost,
                             quint16 nightCost,
                             quint32 dayStartTime,
                             quint32 nightStartTime) override;
};
