QT       += core gui opengl network

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
    functions.cpp \
    game_client.cpp \
    game_level.cpp \
    game_main_class.cpp \
    game_main_functions.cpp \
    game_server.cpp \
    main.cpp \
    onanimation.cpp \
    ongame.cpp \
    player_cube_1.cpp \
    widget.cpp

HEADERS += \
    classes.h \
    defines.h \
    game_additional_classes.h \
    game_client.h \
    game_enums.h \
    game_level.h \
    game_main_class.h \
    game_network_def.h \
    game_network_functions.h \
    game_server.h \
    player_cube_1.h \
    widget.h

RESOURCES += \
    images.qrc \
    font.qrc

FORMS += \
    widget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
