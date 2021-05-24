QT       += core gui
QT       += network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    adder.cpp \
    favorite.cpp \
    favoritebtns.cpp \
    main.cpp \
    mainwindow.cpp \
    obmen.cpp \
    osnova.cpp \
    poisk.cpp \
    registr.cpp

HEADERS += \
    adder.h \
    favorite.h \
    favoritebtns.h \
    mainwindow.h \
    obmen.h \
    osnova.h \
    poisk.h \
    registr.h

FORMS += \
    adder.ui \
    favorite.ui \
    favoritebtns.ui \
    mainwindow.ui \
    osnova.ui \
    poisk.ui \
    registr.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
