#pragma once

#include "AbstractReceiveMessageHandler.h"

#include <QMqttClient>
#include <QJsonArray>

class ReceiveMessageHandler : public AbstractReceiveMessageHandler
{
    QList<QMqttClient*> m_mqtt_clients;
    QJsonArray m_servers;

public:
    ReceiveMessageHandler(QJsonArray servers, QObject *parent = 0);
    ~ReceiveMessageHandler();

    void onRecv(QByteArray data) override;
    void onParkingStatus(quint32 id, quint8 place, bool isFree) override;
};
