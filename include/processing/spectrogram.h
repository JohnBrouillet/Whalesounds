#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>


#define _USE_MATH_DEFINES
#include <cmath>

#include "kiss_fft.h"
#include "filt.h"

#include <iostream>


class Spectrogram : public QObject
{
    Q_OBJECT
public:
    Spectrogram();


public Q_SLOTS:
    void computeFFT(int i, QVector<double> data);
    void setFech(int _fech);

Q_SIGNALS:
    void dataReady(QVector<double>);

private:
    Filter m_HPFilter;
    double fech;

};



#endif // SPECTROGRAM_H
