#-------------------------------------------------
#
# Project created by QtCreator 2011-04-07T18:40:40
#
#-------------------------------------------------

QT       += core gui webkit network opengl

TARGET = ssWebKit
TEMPLATE = app


SOURCES += \
    ssWebKit.cpp \
    webkitwidget.cpp \
    screensaver.cpp  \
    main.cpp



RESOURCES += \
    res.qrc

HEADERS += \
    webkitwidget.h \
    screensaver.h

CONFIG += static
static {
    CONFIG += static
    DEFINES += STATIC
    message("Static build")
}
