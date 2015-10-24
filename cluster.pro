#-------------------------------------------------
#
# Project created by QtCreator 2015-10-18T20:41:39
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = cluster
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += $$(BOOSTDIR)


CONFIG(debug, debug|release) {
    DESTDIR = ../build/debug
} else {
    DESTDIR = ../build/release
}

OBJECTS_DIR = $$DESTDIR/.obj
MOC_DIR = $$DESTDIR/.moc
RCC_DIR = $$DESTDIR/.qrc
UI_DIR = $$DESTDIR/.ui

SOURCES += \
    ClusterFunctions.cpp \
    ClusterSet.cpp \
    computeDBSCAN.cpp \
    DataSet.cpp \
    DataSetUtil.cpp \
    GrahamScan.cpp \
    KMean.cpp \
    Point.cpp \
    Random.cpp \
    cluster.cpp \
    KMeanTest.cpp \
    CommandLine.cpp \
    Sort.cpp

HEADERS += \
    ClusterFunctions.h \
    ClusterSet.h \
    computeDBSCAN.h \
    DataSet.h \
    DataSetUtil.h \
    GrahamScan.h \
    KMean.h \
    Point.h \
    Random.h \
    KMeanTest.h \
    CommandLine.h \
    Sort.h


