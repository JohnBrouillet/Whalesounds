#-------------------------------------------------
#
# Project created by QtCreator 2019-06-17T11:20:56
#
#-------------------------------------------------

QT       += core gui qml quickwidgets multimedia multimediawidgets printsupport quickcontrols2

android{
    QT += androidextras
    QMAKE_CXXFLAGS_RELEASE += -O2
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = whalesound
TEMPLATE = app

ios: {
  QMAKE_TARGET_BUNDLE_PREFIX = com.john
  QMAKE_DEVELOPMENT_TEAM = john
}

msvc: QMAKE_CXXFLAGS_RELEASE += /O2 /arch:AVX
gcc: QMAKE_CXXFLAGS_RELEASE += -O2
clang: QMAKE_CXXFLAGS_RELEASE += -O2


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0



CONFIG += c++11

SOURCES += \
    3rdparties/QRealFourier-master/code/sources/qcomplexnumber.cpp \
    3rdparties/QRealFourier-master/code/sources/qfouriercalculator.cpp \
    3rdparties/QRealFourier-master/code/sources/qfourierfixedcalculator.cpp \
    3rdparties/QRealFourier-master/code/sources/qfouriertransformer.cpp \
    3rdparties/QRealFourier-master/code/sources/qfouriervariablecalculator.cpp \
    3rdparties/QRealFourier-master/code/sources/qwindowfunction.cpp \
    3rdparties/qcustomplot/qcustomplot.cpp \
    src/audio/playercontrols.cpp \
    src/audio/track.cpp \
    src/audio/wavfile.cpp \
    src/gui/abstractPlot.cpp \
    src/gui/audiowidget.cpp \
    src/gui/mainwidget.cpp \
    src/gui/spectrogramWidget.cpp \
    src/main.cpp \
    src/processing/spectrogram.cpp \
    src/utils/jsoncaretaker.cpp \
    src/utils/utils.cpp \
    src/utils/widgetShower.cpp

HEADERS += \
    3rdparties/QRealFourier-master/code/fftreal/Array.h \
    3rdparties/QRealFourier-master/code/fftreal/Array.hpp \
    3rdparties/QRealFourier-master/code/fftreal/DynArray.h \
    3rdparties/QRealFourier-master/code/fftreal/DynArray.hpp \
    3rdparties/QRealFourier-master/code/fftreal/FFTReal.h \
    3rdparties/QRealFourier-master/code/fftreal/FFTReal.hpp \
    3rdparties/QRealFourier-master/code/fftreal/FFTRealFixLen.h \
    3rdparties/QRealFourier-master/code/fftreal/FFTRealFixLen.hpp \
    3rdparties/QRealFourier-master/code/fftreal/FFTRealFixLenParam.h \
    3rdparties/QRealFourier-master/code/fftreal/FFTRealPassDirect.h \
    3rdparties/QRealFourier-master/code/fftreal/FFTRealPassDirect.hpp \
    3rdparties/QRealFourier-master/code/fftreal/FFTRealPassInverse.h \
    3rdparties/QRealFourier-master/code/fftreal/FFTRealPassInverse.hpp \
    3rdparties/QRealFourier-master/code/fftreal/FFTRealSelect.h \
    3rdparties/QRealFourier-master/code/fftreal/FFTRealSelect.hpp \
    3rdparties/QRealFourier-master/code/fftreal/FFTRealUseTrigo.h \
    3rdparties/QRealFourier-master/code/fftreal/FFTRealUseTrigo.hpp \
    3rdparties/QRealFourier-master/code/fftreal/OscSinCos.h \
    3rdparties/QRealFourier-master/code/fftreal/OscSinCos.hpp \
    3rdparties/QRealFourier-master/code/fftreal/def.h \
    3rdparties/QRealFourier-master/code/headers/qcomplexnumber.h \
    3rdparties/QRealFourier-master/code/headers/qfouriercalculator.h \
    3rdparties/QRealFourier-master/code/headers/qfourierfixedcalculator.h \
    3rdparties/QRealFourier-master/code/headers/qfouriertransformer.h \
    3rdparties/QRealFourier-master/code/headers/qfouriervariablecalculator.h \
    3rdparties/QRealFourier-master/code/headers/qwindowfunction.h \
    3rdparties/qcustomplot/qcustomplot.h \
    include/audio/playercontrols.h \
    include/audio/spectrum.h \
    include/audio/track.h \
    include/audio/wavfile.h \
    include/gui/abstractPlot.h \
    include/gui/audiowidget.h \
    include/gui/mainwidget.h \
    include/gui/spectrogramWidget.h \
    include/processing/spectrogram.h \
    include/utils/jsoncaretaker.h \
    include/utils/utils.h \
    include/utils/widgetShower.h


CONFIG += mobility
MOBILITY =


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    data_qrc.qrc \
    icons/icons.qrc

DISTFILES += \
     qml/design.qss

#contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
  #  ANDROID_PACKAGE_SOURCE_DIR = \
 #       $$PWD/android
#}
