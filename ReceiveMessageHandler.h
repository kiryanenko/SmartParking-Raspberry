#pragma once

#include "AbstractReceiveMessageHandler.h"


class ReceiveMessageHandler : public AbstractReceiveMessageHandler
{
public:
    ReceiveMessageHandler();

    void onRecv(const uint8_t *data, size_t size) override;
};
