#ifndef RECEIVEMESSAGEHANDLER_H
#define RECEIVEMESSAGEHANDLER_H
#include <stdint.h>
#include <QByteArray>

class AbstractReceiveMessageHandler
{
public:
    AbstractReceiveMessageHandler();
    virtual ~AbstractReceiveMessageHandler();

    virtual void onRecv(QByteArray data) {}
    virtual void onParkingStatus(uint32_t id) {}
    virtual void onInitStatus(uint16_t period) {}
};

#endif // RECEIVEMESSAGEHANDLER_H
