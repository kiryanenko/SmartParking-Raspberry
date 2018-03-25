#ifndef RECEIVEMESSAGEHANDLER_H
#define RECEIVEMESSAGEHANDLER_H
#include <stdint.h>

class AbstractReceiveMessageHandler
{
public:
    AbstractReceiveMessageHandler();
    virtual ~AbstractReceiveMessageHandler();

    virtual void onRecv(const uint8_t *data, size_t size) {}
    virtual void onParkingStatus(uint32_t id) {}
    virtual void onInitStatus(uint16_t period) {}
};

#endif // RECEIVEMESSAGEHANDLER_H
