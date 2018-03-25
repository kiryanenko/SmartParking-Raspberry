#include "ReceiveMessageHandler.h"

#include <QDebug>

ReceiveMessageHandler::ReceiveMessageHandler() :
    AbstractReceiveMessageHandler()
{
}


void ReceiveMessageHandler::onRecv(QByteArray data)
{
    qDebug() << "[RECV]" << data;
}

void ReceiveMessageHandler::onParkingStatus(quint32 id, quint8 place, bool isFree)
{
    qInfo() << "[PARKING STATUS] ID =" << id << ", place =" << place << ", is free =" << isFree;
}
