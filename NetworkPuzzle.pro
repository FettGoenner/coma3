QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cornertile.cpp \
    endtile.cpp \
    gamemodel.cpp \
    gameview.cpp \
    junctiontile.cpp \
    linetile.cpp \
    main.cpp \
    mainwindow.cpp \
    newgamedialog.cpp \
    tile.cpp

HEADERS += \
    cornertile.h \
    endtile.h \
    gamemodel.h \
    gameview.h \
    junctiontile.h \
    linetile.h \
    mainwindow.h \
    newgamedialog.h \
    tile.h

FORMS += \
    mainwindow.ui \
    newgamedialog.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
