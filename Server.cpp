#include "Server.h"

#include <QDebug>
#include <QJsonObject>
#include <QJsonDocument>

Server::Server(QString host, quint16 mqttPort, QString mqttUsername, QString mqttPwd,
               QString login, QString pwd, QList<quint32> sensors,
               AbstractReceiveMessageHandler *handler, QObject *parent) :
    m_host(host),
    m_mqttPort(mqttPort),
    m_mqttUsername(mqttUsername),
    m_mqttPwd(mqttPwd),
    m_login(login),
    m_pwd(pwd),
    m_sensors(sensors),
    m_handler(handler),
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

void Server::setHandler(AbstractReceiveMessageHandler *handler)
{
    m_handler = handler;
}

void Server::sendParkingStatus(qint64 id, quint8 place, bool isFree)
{
    mqttCheckConnection();

    QJsonObject response = {
        {"login", m_login},
        {"password", m_pwd},
        {"sensor", id},
        {"place_id", place},
        {"free", isFree}
    };
    m_mqtt->publish(QMqttTopicName("status"), QJsonDocument(response).toJson());
}

void Server::sendInit(qint64 id, quint16 samplingPeriod, quint16 sendingPeriod,
                      quint16 dayCost, quint16 nightCost, qint64 dayStartTime, qint64 nightStartTime)
{
    mqttCheckConnection();

    QJsonObject response = {
        {"login", m_login},
        {"password", m_pwd},
        {"sensor", id},
        {"sampling_period", samplingPeriod},
        {"sending_period", sendingPeriod},
        {"day_cost", dayCost},
        {"night_cost", nightCost},
        {"day_start_time", dayStartTime},
        {"night_start_time", nightStartTime}
    };
    m_mqtt->publish(QMqttTopicName("init"), QJsonDocument(response).toJson());
}

void Server::sendPayment(qint64 id, quint8 place, qint64 bookedTime, quint16 payment, quint16 totalCost)
{
    mqttCheckConnection();

    QJsonObject response = {
        {"login", m_login},
        {"password", m_pwd},
        {"sensor", id},
        {"place_id", place},
        {"booked_time", bookedTime},
        {"payment", payment},
        {"total_cost", totalCost}
    };
    m_mqtt->publish(QMqttTopicName("payment"), QJsonDocument(response).toJson());
}

QString Server::bookTopic(quint32 sensor)
{
    return QString("sensor_") + QString::number(sensor) + "-book";
}

QString Server::settingsTopic(quint32 sensor)
{
    return QString("sensor_") + QString::number(sensor) + "-settings";
}


void Server::onConnected()
{
    qInfo() << "MQTT connected to:" << m_host;

    for (auto sensor : m_sensors) {
        auto subscription = m_mqtt->subscribe(QMqttTopicFilter(bookTopic(sensor)));
        if (subscription) {
            qInfo() << "Subscribe to " << subscription->topic().filter();
        } else {
            qCritical() << "[ERROR] Could not subscribe to " << bookTopic(sensor);
        }

        subscription = m_mqtt->subscribe(QMqttTopicFilter(settingsTopic(sensor)));
        if (subscription) {
            qInfo() << "Subscribe to " << subscription->topic().filter();
        } else {
            qCritical() << "[ERROR] Could not subscribe to " << settingsTopic(sensor);
        }
    }

    connect(m_mqtt, SIGNAL(messageReceived(QByteArray, QMqttTopicName)),
            this, SLOT(onMsgRecv(QByteArray, QMqttTopicName)));
}

void Server::onDisconnected()
{
    qCritical() << "[ERROR] MQTT disconnected from:" << m_host;
}

void Server::onMsgRecv(QByteArray msg, QMqttTopicName topic)
{
    qDebug() << "[RECV FROM SERVER] topic:" << topic.name() << "; msg:" << msg;
    try {
        for (auto sensor : m_sensors) {
            if (topic.name() == bookTopic(sensor)) {
                handleRecvBook(sensor, msg);
                return;
            }
            if (topic.name() == settingsTopic(sensor)) {
                handleRecvSettings(sensor, msg);
                return;
            }
        }
    } catch (...) {
        qCritical() << "[ERROR] Error at handle received mqtt msg";
    }

    qWarning() << "[ERROR] Undefined topic:" << topic.name();
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

void Server::handleRecvBook(quint32 sensor, QByteArray data)
{
    auto json = QJsonDocument::fromJson(data);
    m_handler->onBook(sensor,
                      json["place_id"].toInt(),
                      json["booking_time"].toInt());
}

void Server::handleRecvSettings(quint32 sensor, QByteArray data)
{
    auto json = QJsonDocument::fromJson(data);
    m_handler->onSetSensorSettings(sensor,
                                   json["sampling_period"].toInt(),
                                   json["sending_period"].toInt(),
                                   json["day_cost"].toInt(),
                                   json["night_cost"].toInt(),
                                   json["day_start_time"].toInt(),
                                   json["night_start_time"].toInt());
}
