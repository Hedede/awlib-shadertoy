#-------------------------------------------------
#
# Project created by QtCreator 2018-03-21T07:29:35
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = shadertoy
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++1z

SOURCES += \
        main.cpp \
        MainWindow.cpp \
        renderer.cpp \
    Vec2SpinBox.cpp

HEADERS += \
        MainWindow.h \
        renderer.h \
    Vec2SpinBox.h \
    circle.h

unix|win32: LIBS += -lawlog -lawobj -lawio -lawgraphics  -lstdc++fs

DISTFILES += \
    test1.fsh \
    test1.vsh \
    gouraud.fsh \
    gouraud.vsh \
    phong.fsh \
    phong.vsh \
    plane.obj \
    box.obj \
    cylinder.obj \
    arrow.obj \
    blinn.fsh \
    gauss.fsh \
    lambert.fsh \
    cook-torrance.fsh \
    sphere.obj \
    cook-torr.fsh \
    cook-torr.vsh \
    sphere5.obj \
    ambient.vsh

RESOURCES += \
    shaders.qrc
