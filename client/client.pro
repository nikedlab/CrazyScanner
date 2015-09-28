TEMPLATE += app
TARGET = client

DEPENDPATH += .
INCLUDEPATH += .

OBJECTS_DIR = _build
DESTDIR  = ../bin

QT += widgets core gui network
HEADERS += \
    filedialog.h \
    mainwindow.h \
    client.h

SOURCES += \
    main.cpp \
    filedialog.cpp \
    mainwindow.cpp \
    client.cpp

FORMS += \
    filedialog.ui \
    mainwindow.ui
