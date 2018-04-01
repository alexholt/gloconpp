QT += qml quick xml svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gloconpp
TEMPLATE = app

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += $$PWD/main.cpp \
    noiseviewer.cpp

include(../src.pri)

DISTFILES += \
    main.qml \
    assets/icons/app.svg \
    Console.qml

RESOURCES += \
    gloconpp.qrc

CONFIG += c++1z

HEADERS += \
    noiseviewer.h
