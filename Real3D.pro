QT       += core gui
QT       += opengl

CONFIG += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = Real3D
TEMPLATE = app

SOURCES += main.cpp\
    maths/imageprocessing.cpp \
    opengl/camera.cpp \
    opengl/framebuffer.cpp \
    opengl/light.cpp \
    opengl/material.cpp \
    opengl/mesh.cpp \
    opengl/object.cpp \
    opengl/scene.cpp \
    opengl/texture.cpp \
    qt/gldisplay.cpp \
    qt/mainwindow.cpp \
    maths/mathfunctions.cpp \
    other/PFMReadWrite.cpp

HEADERS  += \
    maths/imageprocessing.h \
    opengl/camera.h \
    opengl/framebuffer.h \
    opengl/light.h \
    opengl/material.h \
    opengl/mesh.h \
    opengl/object.h \
    opengl/scene.h \
    opengl/texture.h \
    qt/gldisplay.h \
    qt/mainwindow.h \
    maths/mathfunctions.h \
    opengl/openglheaders.h \
    other/PFMReadWrite.h


FORMS    += mainwindow.ui

win32:{

    INCLUDEPATH += "C:/glew-1.13.0/include"
    LIBS += "C:/glew-1.13.0/lib/Release/x64/glew32.lib"
}
else:unix{
    LIBS += -lGLEW
}

##################### OpenCV   ##############################


win32:{
    CONFIG(debug, debug|release)
    {
         INCLUDEPATH += "C:\\OpenCV2411\\build\\include"

         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_core2411.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_highgui2411.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_imgproc2411.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_features2d2411.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_calib3d2411.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_contrib2411.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_flann2411.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_gpu2411.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_legacy2411.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_ml2411.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_nonfree2411.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_objdetect2411.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_ocl2411.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_photo2411.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_stitching2411.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_superres2411.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_ts2411.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_video2411.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_videostab2411.lib"

    }
    CONFIG(release, debug|release)
    {
         INCLUDEPATH += "C:\\OpenCV2411\\build\\include"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_core2411d.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_highgui2411d.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_imgproc2411d.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_features2d2411d.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_calib3d2411d.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_contrib2411d.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_flann2411d.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_gpu2411d.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_legacy2411d.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_ml2411d.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_nonfree2411d.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_objdetect2411d.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_ocl2411d.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_photo2411d.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_stitching2411d.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_superres2411d.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_ts2411d.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_video2411d.lib"
         LIBS += "C:\\OpenCV2411\\build\\x64\\vc12\\lib\\opencv_videostab2411d.lib"
    }
}
else:unix{
    INCLUDEPATH += /usr/local/include/
    LIBS += -L/usr/local/lib
    LIBS += -lopencv_core
    LIBS += -lopencv_imgproc
    LIBS += -lopencv_highgui
    LIBS += -lopencv_ml
    LIBS += -lopencv_video
    LIBS += -lopencv_features2d
    LIBS += -lopencv_calib3d
    LIBS += -lopencv_objdetect
    LIBS += -lopencv_contrib
    LIBS += -lopencv_legacy
    LIBS += -lopencv_flann
}
