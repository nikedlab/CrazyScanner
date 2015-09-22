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
    clientconnection.h

SOURCES += \
    main.cpp \
    server.cpp \
    fileprocessor.cpp \
    clientconnection.cpp

OTHER_FILES += \
    base.txt

RESOURCES += \
    Resources.qrc

