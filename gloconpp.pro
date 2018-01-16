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
    gamestate.cpp \
    cube.cpp \
    circle.cpp \
    triangle.cpp \

HEADERS += \
    jsconsole.h \
    worldmap.h \
    camera.h \
    glocon.h \
    territory.h \
    pathstate.h \
    renderer.h \
    model.h \
    gamestate.h \
    cube.h \
    circle.h \
    triangle.h \

DISTFILES += \
    main.qml \
    worldmap.vert \
    worldmap.vert \
    assets/shaders/worldmap.frag \
    assets/shaders/worldmap.vert \
    assets/icons/app.svg \
    assets/shaders/cube.frag \
    assets/shaders/cube.vert \
    assets/shaders/cube.frag \
    assets/shaders/cube.vert \
    assets/models/tank.obj \
    README.md \
    assets/textures/marsmap1k.jpg \
    assets/models/mars.fbx \
    assets/models/mars.fbx \
    assets/models/mars.fbx

RESOURCES += \
    gloconpp.qrc

DESTDIR = build

QMAKE_LFLAGS += -v

LIBS += -L/usr/local/lib/ -lassimp
INCLUDEPATH += /usr/local/include
