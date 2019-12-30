#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
#include <QThread>
#include <QQmlContext>
#include <QtQuickWidgets/QQuickWidget>

#include "include/audio/playercontrols.h"
#include "include/processing/spectrogram.h"
#include "include/utils/jsoncaretaker.h"
#include "audiowidget.h"
#include "spectrogramWidget.h"


class MainWidget : public QWidget
{
    Q_OBJECT
public:
    MainWidget(QString path, JsonCaretaker& jsoncare);
    ~MainWidget();

    void setShower(QObject* widget);

public Q_SLOTS:
    void reverseGraph();

private:
    PlayerControls * m_controls;
    AudioWidget * m_audioWidget;
    SpectrogramWidget * m_spectro;
    Spectrogram m_spectrum;
    JsonCaretaker& m_jsoncare;
    QQuickWidget *buttonsWidget;

    QGridLayout * m_grid;
    QThread * m_spectroThread;

    qint64 m_duration;
};

#endif // MAINWIDGET_H
