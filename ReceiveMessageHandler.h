#pragma once

#include "AbstractReceiveMessageHandler.h"
#include "Server.h"

class ReceiveMessageHandler : public AbstractReceiveMessageHandler
{
    QList<Server*> m_servers;

public:
    ReceiveMessageHandler(QList<Server*> &servers, QObject *parent = 0);
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
};
