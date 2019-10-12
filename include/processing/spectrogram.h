#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>


#define _USE_MATH_DEFINES
#include <cmath>

#include "3rdparties/kiss_fft/kiss_fft.h"
#include "3rdparties/filter/filt.h"

#include <iostream>


class Spectrogram : public QObject
{
    Q_OBJECT
public:
    Spectrogram();
    ~Spectrogram();


public Q_SLOTS:
    void computeFFT(int channel, QVector<double> data);
    void setFech();

Q_SIGNALS:
    void dataReady(QVector<double>);

private:
    kiss_fft_cpx* cx_out;
    kiss_fft_cfg cfg;
    Filter m_HPFilter;
    double fech;
    int nbOutputData;

};



#endif // SPECTROGRAM_H
