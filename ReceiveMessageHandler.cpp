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
