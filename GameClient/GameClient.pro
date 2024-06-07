QT       += core gui
QT       += network
QT       += sql
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
TEMPLATE = app

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    applywid.cpp \
    cdwidget.cpp \
    clickedbtn.cpp \
    clickedlabel.cpp \
    customizeedit.cpp \
    friendslist.cpp \
    gamedialog.cpp \
    global.cpp \
    listitembase.cpp \
    logindialog.cpp \
    main.cpp \
    mainwindow.cpp \
    myinputdialog.cpp \
    player.cpp \
    registerdialog.cpp \
    replaywid.cpp \
    resetdialog.cpp \
    sqlitemgr.cpp \
    tcpmgr.cpp \
    timerbtn.cpp \
    userwid.cpp \
    verifycodelabel.cpp

HEADERS += \
    applywid.h \
    cdwidget.h \
    clickedbtn.h \
    clickedlabel.h \
    customizeedit.h \
    friendslist.h \
    gamedialog.h \
    global.h \
    listitembase.h \
    logindialog.h \
    mainwindow.h \
    myinputdialog.h \
    player.h \
    registerdialog.h \
    replaywid.h \
    resetdialog.h \
    sqlitemgr.h \
    tcpmgr.h \
    timerbtn.h \
    userwid.h \
    verifycodelabel.h

FORMS += \
    applywid.ui \
    gamedialog.ui \
    logindialog.ui \
    mainwindow.ui \
    registerdialog.ui \
    replaywid.ui \
    resetdialog.ui \
    userwid.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    rc.qrc
