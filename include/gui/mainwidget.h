#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QSlider>
#include <QLabel>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTime>
#include <QAudioProbe>
#include <QAudioDecoder>
#include <QThread>
#include <QQmlContext>
#include <QPixmap>

#include <QtQuickWidgets/QQuickWidget>

#include "audiowidget.h"
#include "waterfallwidget.h"
#include "include/audio/playercontrols.h"
#include "include/processing/spectrogram.h"
#include "include/utils/jsoncaretaker.h"
#include "include/audio/tonegenerator.h"


class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QString path);

private:
    void engineConnect();

public Q_SLOTS:
    void setFiles(QString specie, QStringList path);

private:
    PlayerControls * m_controls;
    AudioWidget * m_audioWidget;
    WaterfallWidget * m_waterfall;
    Spectrogram m_spectrum;
    QThread * spectroThread;

    JsonCaretaker jsoncare;
    QString dataAbsPath;

    WaveGenerator generator;

    qint64 m_duration;
};

#endif // MAINWIDGET_H
