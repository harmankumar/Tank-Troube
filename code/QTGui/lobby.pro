#-------------------------------------------------
#
# Project created by QtCreator 2015-04-03T23:09:48
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = lobby
TEMPLATE = app


SOURCES += main.cpp\
        lobbywindow.cpp \
         aimode.cpp\
    initialdialog.cpp\
    playmode.cpp



HEADERS  += lobbywindow.h \
           aimode.h \
    initialdialog.h\
    playmode.h



FORMS    += lobbywindow.ui\
            aimode.ui \
    initialdialog.ui \
    playmode.ui


#LIBS += -lsfml-audio -lsfml-graphics -lsfml-network -lsfml-window -lsfml-system -lpthread

#QMAKE_CXXFLAGS += -lGL \
#                  -lglut\
#                  -lGLU \
#                  -std=c++11\
#                  -lpthread\
#                  -w

OTHER_FILES +=

RESOURCES +=

