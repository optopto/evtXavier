#-------------------------------------------------
#
# Project created by QtCreator 2021-11-11T15:38:52
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = evtXavier
TEMPLATE = app
CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp \
        mainxavierevt.cpp \
    presetcamera.cpp \
    configure.cpp \
    ger.cpp \
    diroption.cpp \
    capturedata.cpp \
    gpuinfo.cpp

HEADERS += \
        mainxavierevt.h \
    configure.h \
    emergent.h \
    globalvar.h

FORMS += \
        mainxavierevt.ui \
    configure.ui



CUDA_SOURCES = cuda_test.cu
CUDA_DIR = /usr/local/cuda
CUDA_ARCH = sm_32 # as supported by the Tegra K1
INCLUDEPATH += $$CUDA_DIR/include
LIBS += -L $$CUDA_DIR/lib64 -lcudart -lcuda
osx: LIBS += -F/Library/Frameworks -framework CUDA
cuda.commands = $$CUDA_DIR/bin/nvcc -c -arch=$$CUDA_ARCH -o ${QMAKE_FILE_OUT} ${QMAKE_FILE_NAME}
cuda.dependency_type = TYPE_C
cuda.depend_command = $$CUDA_DIR/bin/nvcc -M ${QMAKE_FILE_NAME}
cuda.input = CUDA_SOURCES
cuda.output = ${QMAKE_FILE_BASE}_cuda.o

QMAKE_EXTRA_COMPILERS += cuda

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/release/ -lEmergentCamera
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/debug/ -lEmergentCamera
else:unix: LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/ -lEmergentCamera

INCLUDEPATH += $$PWD/../../../../../opt/EVT/eSDK/include
DEPENDPATH += $$PWD/../../../../../opt/EVT/eSDK/include


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/release/ -lEmergentCameraC
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/debug/ -lEmergentCameraC
else:unix: LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/ -lEmergentCameraC

INCLUDEPATH += $$PWD/../../../../../opt/EVT/eSDK/include
DEPENDPATH += $$PWD/../../../../../opt/EVT/eSDK/include


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/release/ -lEmergentGenICam
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/debug/ -lEmergentGenICam
else:unix: LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/ -lEmergentGenICam

INCLUDEPATH += $$PWD/../../../../../opt/EVT/eSDK/include
DEPENDPATH += $$PWD/../../../../../opt/EVT/eSDK/include


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/release/ -lEmergentGigEVision
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/debug/ -lEmergentGigEVision
else:unix: LIBS += -L$$PWD/../../../../../opt/EVT/eSDK/lib/ -lEmergentGigEVision

INCLUDEPATH += $$PWD/../../../../../opt/EVT/eSDK/include
DEPENDPATH += $$PWD/../../../../../opt/EVT/eSDK/include




win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/aarch64-linux-gnu/release/ -lopencv_imgproc
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/aarch64-linux-gnu/debug/ -lopencv_imgproc
else:unix: LIBS += -L$$PWD/../../../../../usr/lib/aarch64-linux-gnu/ -lopencv_imgproc

INCLUDEPATH += $$PWD/../../../../../usr/lib/aarch64-linux-gnu
DEPENDPATH += $$PWD/../../../../../usr/lib/aarch64-linux-gnu

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/aarch64-linux-gnu/release/ -lopencv_imgcodecs
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/aarch64-linux-gnu/debug/ -lopencv_imgcodecs
else:unix: LIBS += -L$$PWD/../../../../../usr/lib/aarch64-linux-gnu/ -lopencv_imgcodecs

INCLUDEPATH += $$PWD/../../../../../usr/lib/aarch64-linux-gnu
DEPENDPATH += $$PWD/../../../../../usr/lib/aarch64-linux-gnu

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/aarch64-linux-gnu/release/ -lopencv_highgui
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/aarch64-linux-gnu/debug/ -lopencv_highgui
else:unix: LIBS += -L$$PWD/../../../../../usr/lib/aarch64-linux-gnu/ -lopencv_highgui

INCLUDEPATH += $$PWD/../../../../../usr/lib/aarch64-linux-gnu
DEPENDPATH += $$PWD/../../../../../usr/lib/aarch64-linux-gnu

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/aarch64-linux-gnu/release/ -lopencv_core
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/aarch64-linux-gnu/debug/ -lopencv_core
else:unix: LIBS += -L$$PWD/../../../../../usr/lib/aarch64-linux-gnu/ -lopencv_core

INCLUDEPATH += $$PWD/../../../../../usr/lib/aarch64-linux-gnu
DEPENDPATH += $$PWD/../../../../../usr/lib/aarch64-linux-gnu

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/aarch64-linux-gnu/release/ -lopencv_ml
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../usr/lib/aarch64-linux-gnu/debug/ -lopencv_ml
else:unix: LIBS += -L$$PWD/../../../../../usr/lib/aarch64-linux-gnu/ -lopencv_ml

INCLUDEPATH += $$PWD/../../../../../usr/lib/aarch64-linux-gnu
DEPENDPATH += $$PWD/../../../../../usr/lib/aarch64-linux-gnu

DISTFILES +=






