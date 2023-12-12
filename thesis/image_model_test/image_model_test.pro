QT += testlib
QT += gui
# GUI is needed bc we convert cv::Mat to QT images in the model, so the view doesn't have to interact with opencv

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  tst_imagemodeltest.cpp \
    ../image_editor/ImageModel.cpp \
    ../image_editor/ImagePersistence.cpp \
    ImageModelTests.cpp \
    ImagePersistenceMock.cpp

HEADERS += \
    ../image_editor/ImageData.h \
    ../image_editor/ImageModel.h \
    ../image_editor/ImagePersistence.h \
    ImageModelTests.h \
    ImagePersistenceMock.h

INCLUDEPATH += C:\dev_libs\opencv\build\include

LIBS += C:\dev_libs\opencv-build\bin\libopencv_core481.dll
LIBS += C:\dev_libs\opencv-build\bin\libopencv_highgui481.dll
LIBS += C:\dev_libs\opencv-build\bin\libopencv_imgcodecs481.dll
LIBS += C:\dev_libs\opencv-build\bin\libopencv_imgproc481.dll
LIBS += C:\dev_libs\opencv-build\bin\libopencv_features2d481.dll
LIBS += C:\dev_libs\opencv-build\bin\libopencv_calib3d481.dll
