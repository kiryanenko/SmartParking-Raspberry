#include <QCoreApplication>
#include <QSettings>
#include <QTimer>
#include <QDebug>
#include <QSerialPortInfo>
#include "Worker.h"

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QList<QVariant> defaultSensors;
    defaultSensors << 1 << 2;

    QSettings *settings = new QSettings("settings.ini", QSettings::IniFormat, &a);
    if (settings->value("sensors").isNull()) settings->setValue("sensors", QVariant(defaultSensors));
    if (settings->value("driver").isNull()) settings->setValue("driver", "serial");
    if (settings->value("serial_port").isNull()) settings->setValue("serial_port", QSerialPortInfo().availablePorts()[0].portName());
    if (settings->value("baud_rate").isNull()) settings->setValue("baud_rate", 9600);
    if (settings->value("idle_period").isNull()) settings->setValue("idle_period", 300);
    settings->sync(); //записываем настройки

    Worker *worker = new Worker(settings, &a);
    QTimer *timer = new QTimer(&a);
    QObject::connect(timer, SIGNAL(timeout()), worker, SLOT(run()));
    timer->start(settings->value("idle_period").toInt());

    return a.exec();
}
