#pragma once

#include "AbstractReceiveMessageHandler.h"


class ReceiveMessageHandler : public AbstractReceiveMessageHandler
{
public:
    ReceiveMessageHandler();

    void onRecv(QByteArray data) override;
    void onParkingStatus(quint32 id, quint8 place, bool isFree) override;
};
