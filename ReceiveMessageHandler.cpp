#include "ReceiveMessageHandler.h"

#include <QDebug>
#include <QJsonObject>

ReceiveMessageHandler::ReceiveMessageHandler(QList<Server*> &servers, QObject *parent) :
    m_servers(servers), AbstractReceiveMessageHandler(parent)
{
}

ReceiveMessageHandler::~ReceiveMessageHandler()
{
}


void ReceiveMessageHandler::onRecv(QByteArray data)
{
    qDebug() << "[RECV]" << data;
}

void ReceiveMessageHandler::onParkingStatus(quint32 id, quint8 place, bool isFree)
{
    qInfo() << "[PARKING STATUS] ID =" << id << ", place =" << place << ", is free =" << isFree;
    for (auto serv : m_servers) {
        serv->sendParkingStatus(id, place, isFree);
    }
}

void ReceiveMessageHandler::onInit(quint32 id,
                                         quint16 samplingPeriod,
                                         quint16 sendingPeriod,
                                         quint16 dayCost,
                                         quint16 nightCost,
                                         quint32 dayStartTime,
                                         quint32 nightStartTime)
{
    qInfo() << "[INIT] ID =" << id
            << ", sampling period =" << samplingPeriod
            << ", sending period =" << sendingPeriod
            << ", day cost =" << dayCost
            << ", night cost =" << nightCost
            << ", day start time =" << dayStartTime
            << ", night start time =" << nightStartTime;
    for (auto serv : m_servers) {
        serv->sendInit(id, samplingPeriod, sendingPeriod, dayCost, nightCost, dayStartTime, nightStartTime);
    }
}
