#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QMqttClient>

class Server : QObject
{
    Q_OBJECT

    QString m_host;

    quint16 m_mqttPort;
    QString m_mqttUsername;
    QString m_mqttPwd;

    QString m_login;
    QString m_pwd;

    QMqttClient *m_mqtt;

public:
    Server(QString host, quint16 mqtt_port, QString mqttUsername, QString mqttPwd, QString login, QString pwd, QObject *parent = 0);
    ~Server();

    void sendParkingStatus(qint64 id, quint8 place, bool isFree);
    void sendInit(
            qint64 id,
            quint16 samplingPeriod,
            quint16 sendingPeriod,
            quint16 dayCost,
            quint16 nightCost,
            qint64 dayStartTime,
            qint64 nightStartTime);

public slots:
    void onConnected();
    void onDisconnected();

private:
    void mqttConnect();
    bool mqttCheckConnection();
};

#endif // SERVER_H
