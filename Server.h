#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QMqttClient>
#include "AbstractReceiveMessageHandler.h"


class Server : QObject
{
    Q_OBJECT

    QMqttClient *m_mqtt;

    QString m_host;

    quint16 m_mqttPort;
    QString m_mqttUsername;
    QString m_mqttPwd;

    QString m_login;
    QString m_pwd;

    AbstractReceiveMessageHandler *m_handler;

    QList<quint32> m_sensors;

public:
    Server(QString host, quint16 mqtt_port, QString mqttUsername, QString mqttPwd,
           QString login, QString pwd, QList<quint32> sensors,
           AbstractReceiveMessageHandler *handler = new AbstractReceiveMessageHandler(), QObject *parent = 0);
    ~Server();

    void setHandler(AbstractReceiveMessageHandler *handler);

    void sendParkingStatus(qint64 id, quint8 place, bool isFree);
    void sendInit(qint64 id,
                  quint16 samplingPeriod,
                  quint16 sendingPeriod,
                  quint16 dayCost,
                  quint16 nightCost,
                  qint64 dayStartTime,
                  qint64 nightStartTime);
    void sendPayment(qint64 id, quint8 place, qint64 bookedTime, quint16 payment, quint16 totalCost);

    static QString bookTopic(quint32 sensor);
    static QString settingsTopic(quint32 sensor);

public slots:
    void onConnected();
    void onDisconnected();
    void onMsgRecv(QByteArray msg, QMqttTopicName topic);

private:
    void mqttConnect();
    bool mqttCheckConnection();

    void handleRecvBook(quint32 sensor, QByteArray data);
    void handleRecvSettings(quint32 sensor, QByteArray data);
};

#endif // SERVER_H
