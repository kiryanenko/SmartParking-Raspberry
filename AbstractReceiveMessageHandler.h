#ifndef RECEIVEMESSAGEHANDLER_H
#define RECEIVEMESSAGEHANDLER_H

#include <QtCore>
#include <QByteArray>

class AbstractReceiveMessageHandler : public QObject
{
    Q_OBJECT

public:
    AbstractReceiveMessageHandler(QObject *parent = 0);
    virtual ~AbstractReceiveMessageHandler();

    virtual void onRecv(QByteArray data) {}
    virtual void onParkingStatus(quint32 id, quint8 place, bool isFree) {}
    virtual void onInitStatus(quint16 period) {}
};

#endif // RECEIVEMESSAGEHANDLER_H
