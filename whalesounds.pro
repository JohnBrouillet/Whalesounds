#-------------------------------------------------
#
# Project created by QtCreator 2019-06-17T11:20:56
#
#-------------------------------------------------

QT       += core gui qml quickwidgets multimedia multimediawidgets printsupport quickcontrols2

android{
    QT += androidextras
    DEFINES += AUDIO_ONLY
    QMAKE_CXXFLAGS_RELEASE += -O2
}

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = whalesounds
TEMPLATE = app


msvc: QMAKE_CXXFLAGS_RELEASE += /O2


# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# If defined, algorithms window does not appear.
# Defined by default for Android.
DEFINES += AUDIO_ONLY


CONFIG += c++11

SOURCES += \
    3rdparties/filter/filt.cpp \
    3rdparties/kiss_fft/kiss_fft.c \
    3rdparties/qcustomplot/qcustomplot.cpp \
    src/audio/bufferdecoderthread.cpp \
    src/audio/engine.cpp \
    src/audio/playercontrols.cpp \
    src/audio/tonegenerator.cpp \
    src/audio/wavfile.cpp \
    src/gui/audiowidget.cpp \
    src/gui/mainwidget.cpp \
    src/gui/waterfallwidget.cpp \
    src/main.cpp \
    src/processing/cepstrogramme.cpp \
    src/processing/spectrogram.cpp \
    src/utils/jsoncaretaker.cpp \
    src/utils/utils.cpp

HEADERS += \
    3rdparties/filter/filt.h \
    3rdparties/kiss_fft/_kiss_fft_guts.h \
    3rdparties/kiss_fft/kiss_fft.h \
    3rdparties/kiss_fft/kissfft.hh \
    3rdparties/qcustomplot/qcustomplot.h \
    include/audio/bufferdecoderthread.h \
    include/audio/engine.h \
    include/audio/playercontrols.h \
    include/audio/spectrum.h \
    include/audio/tonegenerator.h \
    include/audio/wavfile.h \
    include/gui/audiowidget.h \
    include/gui/mainwidget.h \
    include/gui/waterfallwidget.h \
    include/processing/cepstrogramme.h \
    include/processing/spectrogram.h \
    include/utils/jsoncaretaker.h \
    include/utils/utils.h





CONFIG += mobility
MOBILITY =


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    data_qrc.qrc \
    icons/icons.qrc \
    imagine-assets.qrc

DISTFILES += \
    android/AndroidManifest.xml \
    android/build.gradle \
    android/gradle/wrapper/gradle-wrapper.jar \
    android/gradle/wrapper/gradle-wrapper.properties \
    android/gradlew \
    android/gradlew.bat \
    android/res/values/libs.xml \
    qml/design.qss \
    qtquickcontrols2.conf

contains(ANDROID_TARGET_ARCH,armeabi-v7a) {
    ANDROID_PACKAGE_SOURCE_DIR = \
        $$PWD/android
}
