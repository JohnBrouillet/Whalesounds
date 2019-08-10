#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QSharedPointer>
#include <QCheckBox>
#include <QSlider>

#include "kiss_fft.h"
#include "include/gui/qcustomplot.h"
#include "filt.h"

#include <iostream>


class Spectrogram : public QObject
{
    Q_OBJECT
public:
    Spectrogram();

public Q_SLOTS:
    void computeFFT(int i, QVector<double> data);

Q_SIGNALS:
    void dataReady(QVector<double>);

private:
    Filter m_HPFilter;

};

class SpectrogramWidget : public QWidget
{
    Q_OBJECT
public:
    SpectrogramWidget();

    void plotDesign();

public Q_SLOTS:
    void plotData(QVector<qreal> data);
    void setFech(int _fech);
    void zoom(bool X, bool Y);

    void zoomActivation();
    void zoomSpectro(bool value);
    void changeColorRange(bool value);

    void reinit();

private:
    QCustomPlot * plot;
    QCPColorMap * colorMap;
    QCPColorScale *colorScale;

    QVector<double> xAxis;

    bool zoomX, zoomY;

};

#endif // SPECTROGRAM_H
