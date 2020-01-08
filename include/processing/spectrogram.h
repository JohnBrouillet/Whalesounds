#ifndef SPECTROGRAM_H
#define SPECTROGRAM_H

#include <QObject>

#define _USE_MATH_DEFINES
#include <cmath>

#include "3rdparties/QRealFourier-master/code/headers/qfouriertransformer.h"

class Spectrogram : public QObject
{
    Q_OBJECT
public:
    Spectrogram();

    void fft(QVector<double>& data, bool padding);

public Q_SLOTS:
    void computeFFT();

Q_SIGNALS:
    void dataReady(QVector<double>);

private:
    QVector<double> m_out;
    QFourierTransformer m_transformer;
};



#endif // SPECTROGRAM_H
