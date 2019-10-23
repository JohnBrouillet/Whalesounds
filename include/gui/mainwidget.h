#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSlider>
#include <QLabel>
#include <QTime>

#include <QThread>
#include <QQmlContext>
#include <QPixmap>

#include <QtQuickWidgets/QQuickWidget>

#include "include/audio/playercontrols.h"
#include "include/processing/spectrogram.h"
#include "include/utils/jsoncaretaker.h"
#include "audiowidget.h"
#include "spectrogramWidget.h"

class MainWidget : public QWidget
{

public:
    MainWidget(QString path);
    ~MainWidget();

private:
    void engineConnect();

private:
    PlayerControls * m_controls;
    AudioWidget * m_audioWidget;
    SpectrogramWidget * m_spectro;
    Spectrogram m_spectrum;

    QThread * m_spectroThread;

    JsonCaretaker m_jsoncare;

    QString m_dataAbsPath;

    qint64 m_duration;
};

#endif // MAINWIDGET_H
