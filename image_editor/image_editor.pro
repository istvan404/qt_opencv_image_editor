QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ImageModel.cpp \
    ImagePersistence.cpp \
    main.cpp \
    ImageView.cpp

HEADERS += \
    ImageData.h \
    ImageModel.h \
    ImagePersistence.h \
    ImageView.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += C:\dev_libs\opencv\build\include

LIBS += C:\dev_libs\opencv-build\bin\libopencv_core481.dll
LIBS += C:\dev_libs\opencv-build\bin\libopencv_highgui481.dll
LIBS += C:\dev_libs\opencv-build\bin\libopencv_imgcodecs481.dll
LIBS += C:\dev_libs\opencv-build\bin\libopencv_imgproc481.dll
LIBS += C:\dev_libs\opencv-build\bin\libopencv_features2d481.dll
LIBS += C:\dev_libs\opencv-build\bin\libopencv_calib3d481.dll
