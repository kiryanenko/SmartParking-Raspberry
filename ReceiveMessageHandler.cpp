#include "ReceiveMessageHandler.h"

#include <QDebug>
#include <QJsonObject>

ReceiveMessageHandler::ReceiveMessageHandler(QJsonArray servers, QObject *parent) :
    m_servers(servers), AbstractReceiveMessageHandler(parent)
{
    for (QJsonValue  server : servers) {
        qInfo() << "Connect to " << server["host"].toString();
        QMqttClient *client = new QMqttClient(this);
        client->setHostname(server["host"].toString());
        client->setPort(server["mqtt_port"].toInt());
        client->setUsername(server["mqtt_username"].toString());
        client->setPassword(server["mqtt_password"].toString());
        client->connectToHost();
        m_mqtt_clients << client;
    }
}

ReceiveMessageHandler::~ReceiveMessageHandler()
{
    for (QMqttClient *client : m_mqtt_clients) {
        client->disconnect();
    }
}


void ReceiveMessageHandler::onRecv(QByteArray data)
{
    qDebug() << "[RECV]" << data;
}

void ReceiveMessageHandler::onParkingStatus(quint32 id, quint8 place, bool isFree)
{
    qInfo() << "[PARKING STATUS] ID =" << id << ", place =" << place << ", is free =" << isFree;
    for (int i = 0; i < m_servers.count(); i++) {
        qint64 _id = id;
        QJsonObject response = {
            {"login", m_servers[i].toObject()["login"]},
            {"password", m_servers[i].toObject()["password"]},
            {"sensor", _id},
            {"place_id", place},
            {"free", isFree}
        };
        m_mqtt_clients[i]->publish(QMqttTopicName("status"), QJsonDocument(response).toJson());
    }
}
