QT += testlib
QT += gui
# GUI is needed bc we convert cv::Mat to QT images in the model, so the view doesn't have to interact with opencv

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  \
    ../image_editor/ImageModel.cpp \
    ImageModelTests.cpp

HEADERS += \
    ../image_editor/ImageData.h \
    ../image_editor/ImageModel.h \
    ImageModelTests.h

INCLUDEPATH += C:\dev_libs\opencv\build\include

LIBS += C:\dev_libs\opencv-build\bin\libopencv_core481.dll
LIBS += C:\dev_libs\opencv-build\bin\libopencv_highgui481.dll
LIBS += C:\dev_libs\opencv-build\bin\libopencv_imgcodecs481.dll
LIBS += C:\dev_libs\opencv-build\bin\libopencv_imgproc481.dll
LIBS += C:\dev_libs\opencv-build\bin\libopencv_features2d481.dll
LIBS += C:\dev_libs\opencv-build\bin\libopencv_calib3d481.dll
