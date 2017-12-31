QT += qml quick xml svg

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = gloconpp
TEMPLATE = app

DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    jsconsole.cpp \
    worldmap.cpp \
    camera.cpp \
    glocon.cpp \
    territory.cpp \
    renderer.cpp \
    model.cpp \
    gamestate.cpp

HEADERS += \
    jsconsole.h \
    worldmap.h \
    camera.h \
    glocon.h \
    territory.h \
    pathstate.h \
    renderer.h \
    model.h \
    gamestate.h

DISTFILES += \
    main.qml \
    worldmap.vert \
    worldmap.vert \
    assets/shaders/worldmap.frag \
    assets/shaders/worldmap.vert \
    assets/icons/app.svg

RESOURCES += \
    gloconpp.qrc

DESTDIR = build

QMAKE_LFLAGS += -v
