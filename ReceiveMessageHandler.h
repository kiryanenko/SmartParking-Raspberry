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
};
