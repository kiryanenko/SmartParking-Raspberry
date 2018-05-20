#include "ReceiveMessageHandler.h"

#include <QDebug>
#include <QJsonObject>

ReceiveMessageHandler::ReceiveMessageHandler(QList<Server*> &servers, Driver *driver, QObject *parent) :
    m_servers(servers), m_driver(driver), AbstractReceiveMessageHandler(parent)
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

void ReceiveMessageHandler::onPayment(quint32 id, quint8 place, quint32 bookedTime, quint16 payment, quint16 totalCost)
{
    qInfo() << "[PAYMENT TERMINAL] ID =" << id
            << ", place ID =" << place
            << ", booked time =" << bookedTime
            << ", payment =" << payment
            << ", total cost =" << totalCost;
    for (auto serv : m_servers) {
        serv->sendPayment(id, place, bookedTime, payment, totalCost);
    }
}

void ReceiveMessageHandler::onBook(quint32 sensor, quint8 place, quint32 bookedTime)
{
    qInfo() << "[BOOK] ID =" << sensor
            << ", place ID =" << place
            << ", booked time =" << bookedTime;
    m_driver->sendReserve(sensor, place, bookedTime);
}

void ReceiveMessageHandler::onSetSensorSettings(quint32 sensor, quint16 samplingPeriod, quint16 sendingPeriod,
                                                quint16 dayCost, quint16 nightCost, quint32 dayStartTime, quint32 nightStartTime)
{
    qInfo() << "[SET SENSOR SETTINGS] ID =" << sensor
            << ", sampling period =" << samplingPeriod
            << ", sending period =" << sendingPeriod
            << ", day cost =" << dayCost
            << ", night cost =" << nightCost
            << ", day start time =" << dayStartTime
            << ", night start time =" << nightStartTime;
    m_driver->sendSetSettings(sensor, samplingPeriod, sendingPeriod, dayCost, nightCost, dayStartTime, nightStartTime);
}
