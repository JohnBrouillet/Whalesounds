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
#include "include/audio/playercontrols.h"
#include "include/processing/spectrogram.h"
#include "include/utils/jsoncaretaker.h"
#include "include/audio/tonegenerator.h"


class MainWidget : public QWidget
{
    Q_OBJECT

public:
    MainWidget(QString path);

    void updateDurationInfo(qint64 currentInfo);

private:
    void engineConnect();

public Q_SLOTS:
    void seek(int seconds);
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void openFileNameReady();
    void setFiles(QStringList path);
    void setImage(QString path);

private:
    PlayerControls * m_controls;
    AudioWidget * m_audioWidget;
    SpectrogramWidget * m_spectrogram;
    Spectrogram m_spectrum;
    QThread * spectroThread;

    JsonCaretaker jsoncare;
    QString dataAbsPath;

    WaveGenerator generator;

    qint64 m_duration;
};

#endif // MAINWIDGET_H
