SOURCES += \
    $$PWD/src/jsconsole.cpp \
    $$PWD/src/worldmap.cpp \
    $$PWD/src/camera.cpp \
    $$PWD/src/territory.cpp \
    $$PWD/src/renderer.cpp \
    $$PWD/src/model.cpp \
    $$PWD/src/gamestate.cpp \
    $$PWD/src/cube.cpp \
    $$PWD/src/circle.cpp \
    $$PWD/src/triangle.cpp \
    $$PWD/src/scene.cpp \
    $$PWD/src/edge.cpp \
    $$PWD/src/rect.cpp

HEADERS += \
    $$PWD/src/jsconsole.h \
    $$PWD/src/worldmap.h \
    $$PWD/src/camera.h \
    $$PWD/src/glocon.h \
    $$PWD/src/territory.h \
    $$PWD/src/pathstate.h \
    $$PWD/src/renderer.h \
    $$PWD/src/model.h \
    $$PWD/src/gamestate.h \
    $$PWD/src/cube.h \
    $$PWD/src/circle.h \
    $$PWD/src/triangle.h \
    $$PWD/src/scene.h \
    $$PWD/src/edge.h \
    $$PWD/src/rect.h

DESTDIR = ../build

QMAKE_LFLAGS += -v

LIBS += -L/usr/local/lib/ -lassimp
INCLUDEPATH += /usr/local/include

OBJECTS_DIR = ../build/obj
MOC_DIR = ../build/moc
RCC_DIR = ../build/rcc

DISTFILES += \
    $$PWD/assets/shaders/diffuse.frag \
    $$PWD/assets/shaders/diffuse.vert \
    $$PWD/assets/shaders/ads.vert
