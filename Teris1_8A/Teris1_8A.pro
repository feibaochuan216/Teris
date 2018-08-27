#-------------------------------------------------
#
# Project created by QtCreator 2018-08-25T10:47:36
#
#-------------------------------------------------

QT       += core gui widgets

TARGET = Teris1_8A
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

SOURCES += \
        main.cpp \
    exception/Exception.cpp \
    exception/GeomException.cpp \
    exception/NullParentException.cpp \
    exception/NullPtrException.cpp \
    exception/OutRangeException.cpp \
    exception/OutRangeGeomException.cpp \
    lib/Mask.cpp \
    lib/Tools.cpp \
    service/GamePanel.cpp \
    service/Lattice.cpp \
    service/NextPanel.cpp \
    service/Service.cpp \
    service/Shape.cpp \
    show/GameDlg.cpp \
    show/SetDlg.cpp \
    config.cpp

HEADERS += \
    exception/Exception.h \
    exception/GeomException.h \
    exception/NullParentException.h \
    exception/NullPtrException.h \
    exception/OutRangeException.h \
    exception/OutRangeGeomException.h \
    lib/Mask.h \
    lib/Tools.h \
    service/GamePanel.h \
    service/Lattice.h \
    service/NextPanel.h \
    service/Service.h \
    service/Shape.h \
    show/GameDlg.h \
    show/SetDlg.h \
    config.h

FORMS += \
        GameDlg.ui \
    SetDlg.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
