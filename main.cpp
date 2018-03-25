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

    QSettings *settings = new QSettings("settings.ini", QSettings::IniFormat, &a);
    if (settings->value("idle_period").isNull()) settings->setValue("idle_period", 300);
    if (settings->value("driver").isNull()) settings->setValue("driver", "serial");
    if (settings->value("serial_port").isNull()) settings->setValue("serial_port", QSerialPortInfo().availablePorts()[0].portName());
    settings->sync(); //записываем настройки

    Worker *worker = new Worker(settings, &a);
    QTimer *timer = new QTimer(&a);
    QObject::connect(timer, SIGNAL(timeout()), worker, SLOT(run()));
    timer->start(settings->value("idle_period").toInt());

    return a.exec();
}
