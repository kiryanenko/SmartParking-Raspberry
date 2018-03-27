#include "Server.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

Server::Server(QString host, quint16 mqttPort, QString mqttUsername, QString mqttPwd, QString login, QString pwd, QObject *parent) :
    m_host(host),
    m_mqttPort(mqttPort),
    m_mqttUsername(mqttUsername),
    m_mqttPwd(mqttPwd),
    m_login(login),
    m_pwd(pwd),
    QObject(parent)
{
    m_mqtt = new QMqttClient(this);
    m_mqtt->setHostname(host);
    m_mqtt->setPort(mqttPort);
    m_mqtt->setUsername(mqttUsername);
    m_mqtt->setPassword(mqttPwd);

    connect(m_mqtt, SIGNAL(connected()), this, SLOT(onConnected()));
    connect(m_mqtt, SIGNAL(disconnected()), this, SLOT(onDisconnected()));

    mqttConnect();
}

Server::~Server()
{
    m_mqtt->disconnect();
}

void Server::sendParkingStatus(quint32 id, quint8 place, bool isFree)
{
    mqttCheckConnection();

    qint64 _id = id;
    QJsonObject response = {
        {"login", m_login},
        {"password", m_pwd},
        {"sensor", _id},
        {"place_id", place},
        {"free", isFree}
    };
    m_mqtt->publish(QMqttTopicName("status"), QJsonDocument(response).toJson());
}

void Server::onConnected()
{
    qInfo() << "MQTT connected to:" << m_host;
}

void Server::onDisconnected()
{
    qCritical() << "[ERROR] MQTT disconnected from:" << m_host;
    mqttConnect();
}

void Server::mqttConnect()
{
    qInfo() << "MQTT connect to " << m_host;
    m_mqtt->connectToHost();
}

bool Server::mqttCheckConnection()
{
    if (m_mqtt->state() != QMqttClient::Connected) {
        qCritical() << "[ERROR] MQTT not connected to:" << m_host;
        mqttConnect();
        return false;
    }
    return true;
}
