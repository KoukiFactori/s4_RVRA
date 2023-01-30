#-------------------------------------------------
#
# Project created by QtCreator 2017
#
# Ne pas modifier

QT       += widgets gui opengl

TARGET = AS2MViewer
CONFIG   += console opengl
CONFIG   -= app_bundle

TEMPLATE = app

OBJECTS_DIR = ./obj
MOC_DIR     = ./obj
LIBS +=  -lopengl32

SOURCES += ./src/main.cpp \
           ./src/AS2MWidget.cpp

HEADERS += \
           ./src/AS2MWidget.h
