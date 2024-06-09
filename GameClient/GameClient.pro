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
    src/applywid.cpp \
    src/cdwidget.cpp \
    src/clickedbtn.cpp \
    src/clickedlabel.cpp \
    src/customizeedit.cpp \
    src/friendslist.cpp \
    src/gamedialog.cpp \
    src/global.cpp \
    src/listitembase.cpp \
    src/logindialog.cpp \
    src/main.cpp \
    src/mainwindow.cpp \
    src/myinputdialog.cpp \
    src/player.cpp \
    src/registerdialog.cpp \
    src/replaywid.cpp \
    src/replyitem.cpp \
    src/resetdialog.cpp \
    src/sqlitemgr.cpp \
    src/tcpmgr.cpp \
    src/timerbtn.cpp \
    src/userwid.cpp \
    src/verifycodelabel.cpp

FORMS += \
    inc/logindialog.ui \
    inc/registerdialog.ui \
    ui/gamedialog.ui \
    ui/logindialog.ui \
    ui/logindialog.ui \
    ui/registerdialog.ui \
    ui/registerdialog.ui \
    ui/applywid.ui \
    ui/gamedialog.ui \
    ui/logindialog.ui \
    ui/mainwindow.ui \
    ui/registerdialog.ui \
    ui/replaywid.ui \
    ui/replyitem.ui \
    ui/resetdialog.ui \
    ui/userwid.ui

HEADERS += \
    inc/applywid.h \
    inc/cdwidget.h \
    inc/clickedbtn.h \
    inc/clickedlabel.h \
    inc/customizeedit.h \
    inc/friendslist.h \
    inc/gamedialog.h \
    inc/gamedialog.h \
    inc/global.h \
    inc/listitembase.h \
    inc/logindialog.h \
    inc/mainwindow.h \
    inc/myinputdialog.h \
    inc/player.h \
    inc/registerdialog.h \
    inc/replaywid.h \
    inc/replyitem.h \
    inc/resetdialog.h \
    inc/sqlitemgr.h \
    inc/tcpmgr.h \
    inc/timerbtn.h \
    inc/userwid.h \
    inc/verifycodelabel.h

RESOURCES += \
    rc.qrc
