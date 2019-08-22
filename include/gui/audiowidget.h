#ifndef AUDIOWIDGET_H
#define AUDIOWIDGET_H

#include <QWidget>

#include <QAudioBuffer>

#include <cmath>
#include <iostream>

#include "3rdparties/qcustomplot/qcustomplot.h"

class AudioWidget : public QWidget
{
    Q_OBJECT

public:
    AudioWidget();

    void getBuffer(const QAudioBuffer &buffer);

public Q_SLOTS:
    void dataLengthChanged(qint64 value);
    void getData(int serieIdx, QVector<double> data);
    void channelCountChanged(int value);

private:
    void plotDesign();

private:
    QCustomPlot* plot;
    QVector<double> xAxis;

    int sampleCount;
    int channelCount;
    bool init;
};

#endif // AUDIOWIDGET_H
