QT       += core gui network
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

SOURCES += \
    Metrodatabasehandler.cpp \
    farepage.cpp \
    loginpage.cpp \
    main.cpp \
    mainwindow.cpp \
    routeplanner.cpp \
    signuppage.cpp \
    welcomepage.cpp

HEADERS += \
    Metrodatabasehandler.h \
    farepage.h \
    loginpage.h \
    mainwindow.h \
    routeplanner.h \
    signuppage.h \
    welcomepage.h

FORMS += \
    mainwindow.ui
