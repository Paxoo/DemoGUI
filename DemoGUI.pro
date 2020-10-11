QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    GUI/Data/Player/player.cpp \
    GUI/Data/Player/playerInfo.cpp \
    GUI/Data/Player/playerStats.cpp \
    GUI/Data/match.cpp \
    GUI/Data/parser.cpp \
    GUI/Data/round.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    GUI/Data/Player/player.h \
    GUI/Data/Player/playerInfo.h \
    GUI/Data/Player/playerStats.h \
    GUI/Data/match.h \
    GUI/Data/parser.h \
    GUI/Data/round.h \
    mainwindow.h \

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
