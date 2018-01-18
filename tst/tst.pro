QT += testlib qml quick xml svg
QT -= gui

TARGET = test
CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_triangletest.cpp \
    tst_territorytest.cpp \
    testmain.cpp \
    tst_circletest.cpp

DESTDIR = ../build
OBJECTS_DIR = ../build/obj
MOC_DIR = ../build/moc

INCLUDEPATH += ../src

include(../src.pri)

HEADERS += \
    tst_circletest.h \
    tst_territorytest.h \
    tst_triangletest.h
