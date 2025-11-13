QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    applianceGenerator.cpp \
    buffer.cpp \
    logger.cpp \
    main.cpp \
    mainwindow.cpp \
    product.cpp \
    productioncontroller.cpp \
    station.cpp

HEADERS += \
    applianceGenerator.h \
    assembler.h \
    buffer.h \
    labeler.h \
    logger.h \
    mainwindow.h \
    packager.h \
    product.h \
    productioncontroller.h \
    station.h \
    storage.h \
    tester.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
