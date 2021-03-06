QT       += core gui widgets sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BaseTable.cpp \
    Model.cpp \
    addwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    searchwindow.cpp \
    viewwindow.cpp

HEADERS += \
    BaseTable.h \
    DateStruct.h \
    IncludeLib.h \
    Model.h \
    SqlTable.h \
    Table.h \
    addwindow.h \
    mainwindow.h \
    searchwindow.h \
    viewwindow.h

FORMS += \
    addwindow.ui \
    mainwindow.ui \
    searchwindow.ui \
    viewwindow.ui

INCLUDEPATH += \
    /Users/dmitr/Documents/C++/Boost/boost_1_76_0

LIBS += \
    -L/Users/dmitr/Documents/C++/Boost/boost_1_76_0/stage/lib

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
