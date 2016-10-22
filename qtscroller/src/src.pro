TEMPLATE = lib
CONFIG += staticlib
DEPENDPATH += ../include
INCLUDEPATH += ../include
TARGET = QtScroller
DESTDIR = ../lib

QT += webkit

# Input
HEADERS += qtflickgesture_p.h \
           qtscroller.h \
           qtscroller_p.h \
           qtscrollerfilter_p.h \
           qtscrollerproperties.h \
           qtscrollerproperties_p.h \
           qtscrollevent.h \
           qtscrollevent_p.h
SOURCES += qtflickgesture.cpp \
           qtscroller.cpp \
           qtscrollerfilter.cpp \
           qtscrollerproperties.cpp \
           qtscrollevent.cpp
