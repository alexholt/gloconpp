QT += qml quick xml svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gloconpp
TEMPLATE = app

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += $$PWD/main.cpp

include(../src.pri)

DISTFILES += \
    main.qml \
    assets/icons/app.svg \
    assets/models/tank.obj \
    assets/textures/marsmap1k.jpg \
    assets/models/mars.fbx \
    assets/models/mars.fbx \
    assets/models/mars.fbx

RESOURCES += \
    gloconpp.qrc

CONFIG += c++1z
