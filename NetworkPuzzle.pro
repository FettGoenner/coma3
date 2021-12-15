QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    cornertilemodel.cpp \
    dockwindow.cpp \
    endtilemodel.cpp \
    gamemodel.cpp \
    gameview.cpp \
    junctiontilemodel.cpp \
    linetilemodel.cpp \
    main.cpp \
    mainwindowview.cpp \
    newgamedialog.cpp \
    savegameview.cpp \
    tilemodel.cpp \
    tileview.cpp

HEADERS += \
    cornertilemodel.h \
    dockwindow.h \
    endtilemodel.h \
    gamemodel.h \
    gameview.h \
    junctiontilemodel.h \
    linetilemodel.h \
    mainwindowview.h \
    newgamedialog.h \
    savegameview.h \
    tilemodel.h \
    tileview.h

FORMS += \
    dockwindow.ui \
    mainwindowview.ui \
    newgamedialog.ui \
    savegameview.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
