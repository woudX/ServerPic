TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    global.cpp \
    zextractor.cpp \
    filesys.cpp \
    convert.cpp \
    assist.cpp \
    opencvex.cpp \
    jsonx.cpp \
    createproc.cpp \
    function.cpp \
    network.cpp \
    cthread.cpp \
    simpleapp.cpp \
    serverpic.cpp \
    commands.cpp \
    commandmgr.cpp
INCLUDEPATH = /usr/local/include/opencv
LIBS += -L/usr/local/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_core -lopencv_hal 

INCLUDEPATH += /usr/local/include/json-c
LIBS += -L/usr/local/lib -ljson-c

INCLUDEPATH += /usr/local/include
LIBS += -L/usr/local/lib -lcurl
LIBS += -L /lib64 -l pthread

HEADERS += \
    stdafx.h \
    global.h \
    zextractor.h \
    filesys.h \
    convert.h \
    assist.h \
    opencvex.h \
    jsonx.h \
    createproc.h \
    function.h \
    network.h \
    cthread.h \
    simpleapp.h \
    serverpic.h \
    commands.h \
    commandmgr.h \
