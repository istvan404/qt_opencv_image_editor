# Created by and for Qt Creator This file was created for editing the project sources only.
# You may attempt to use it for building too, by modifying this file here.

#TARGET = image_editor

QT = core gui widgets

HEADERS = \
   $$PWD/Adjustment.h \
   $$PWD/ImageData.h \
   $$PWD/ImageModel.h \
   $$PWD/ImagePersistence.h \
   $$PWD/ImageView.h

SOURCES = \
   $$PWD/Adjustment.cpp \
   $$PWD/ImageModel.cpp \
   $$PWD/ImagePersistence.cpp \
   $$PWD/ImageView.cpp \
   $$PWD/main.cpp

INCLUDEPATH = \
    $$PWD/.

#DEFINES = 

INCLUDEPATH += C:\dev_libs\opencv\build\include

LIBS += C:\dev_libs\opencv-build\bin\libopencv_core481.dll
LIBS += C:\dev_libs\opencv-build\bin\libopencv_highgui481.dll
LIBS += C:\dev_libs\opencv-build\bin\libopencv_imgcodecs481.dll
LIBS += C:\dev_libs\opencv-build\bin\libopencv_imgproc481.dll
LIBS += C:\dev_libs\opencv-build\bin\libopencv_features2d481.dll
LIBS += C:\dev_libs\opencv-build\bin\libopencv_calib3d481.dll
