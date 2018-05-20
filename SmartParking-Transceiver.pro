QT -= gui
QT += serialport
QT += mqtt
QT += core

CONFIG += c++11 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS RASPBERRY_PI BCM2835_NO_DELAY_COMPATIBILITY

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += libs/RadioHead

SOURCES += main.cpp \
    SerialConnection.cpp \
    AbstractReceiveMessageHandler.cpp \
    ReceiveMessageHandler.cpp \
    Driver.cpp \
    Worker.cpp \
    Server.cpp \
    LoRaConnection.cpp \
    libs/RadioHead/RHutil/RasPi.cpp \
    libs/RadioHead/RH_RF95.cpp \
    libs/RadioHead/RHDatagram.cpp \
    libs/RadioHead/RHGenericDriver.cpp \
    libs/RadioHead/RHGenericSPI.cpp \
    libs/RadioHead/RHHardwareSPI.cpp \
    libs/RadioHead/RHSPIDriver.cpp \


HEADERS += \
    SerialConnection.h \
    AbstractReceiveMessageHandler.h \
    ReceiveMessageHandler.h \
    Driver.h \
    Worker.h \
    Server.h \
    LoRaConnection.h \
    libs/RadioHead/RHutil/RasPi.h \
    libs/RadioHead/RH_RF95.h \
    libs/RadioHead/RHDatagram.h \
    libs/RadioHead/RHGenericDriver.h \
    libs/RadioHead/RHGenericSPI.h \
    libs/RadioHead/RHHardwareSPI.h \
    libs/RadioHead/RHSPIDriver.h \


LIBS += -lbcm2835

target.path     = /home/pi/SmartParking-Transceiver
INSTALLS        = target
