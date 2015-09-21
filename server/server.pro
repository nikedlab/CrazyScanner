TEMPLATE += app
TARGET = server
DEPENDPATH += .
INCLUDEPATH += .

OBJECTS_DIR = _build
DESTDIR  = ../bin

QT += network widgets core gui concurrent
HEADERS += \
    main.h \
    server.h \
    fileprocessor.h \
    clientthread.h

SOURCES += \
    main.cpp \
    server.cpp \
    fileprocessor.cpp \
    clientthread.cpp

OTHER_FILES += \
    base.txt

RESOURCES += \
    Resources.qrc

