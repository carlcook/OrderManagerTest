#-------------------------------------------------
#
# Project created by QtCreator 2015-03-07T10:51:00
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = firsttest
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

TEMPLATE = app


SOURCES += \
    eurexModule.cc \
    emlServer.cc \
    main.cc

HEADERS += \
    types.h \
    eurexModule.h \
    emlServer.h
