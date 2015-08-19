TEMPLATE += app
TARGET = server
DEPENDPATH += .
INCLUDEPATH += .

OBJECTS_DIR = _build
DESTDIR  = ../bin

QT += network widgets core gui
HEADERS += \
    main.h \
    server.h \
    fileprocessor.h

SOURCES += \
    main.cpp \
    server.cpp \
    fileprocessor.cpp

OTHER_FILES += \
    base.txt

RESOURCES += \
    Resources.qrc

