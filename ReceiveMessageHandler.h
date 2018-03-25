#pragma once

#include "AbstractReceiveMessageHandler.h"


class ReceiveMessageHandler : public AbstractReceiveMessageHandler
{
public:
    ReceiveMessageHandler();

    void onRecv(QByteArray data) override;
};
