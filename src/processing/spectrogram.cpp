#include "include/processing/spectrogram.h"
#include "include/audio/spectrum.h"
#include "include/audio/track.h"

#include <QVector>
#include <numeric>
#include <iostream>

Spectrogram::Spectrogram()
{
    m_transformer.setSize(NFFT);
    m_transformer.setWindowFunction("Hamming");
}

void Spectrogram::fft(QVector<double> data)
{
    int nbData = data.size();
    if(nbData < NFFT)
    {
        for(int i = 0; i < NFFT - nbData; i++)
            data.push_back(0.0);
    }

    double fft[NFFT];

    m_transformer.forwardTransform(data.data(), fft);

    for(int i = 0; i < NFFT / 2; i++)
        m_out.push_back(20*std::abs(std::log(std::abs(fft[i]) * 1e6)));
}

void Spectrogram::computeFFT()
{
    m_out.clear();
    std::vector<qreal> data = Track::get()->getData()[0].toStdVector();
    double overlapPerc = 0.5;

    int chunkSize = temporalResolution * Track::get()->getFormat().sampleRate();
    int overlap = chunkSize * overlapPerc;

    std::vector<qreal> tmp(data.begin(), data.begin() + chunkSize);
    QVector<qreal> buf = QVector<qreal>::fromStdVector(tmp);

    fft(buf);

    for(int i = 1; i < (data.size() / chunkSize); i++)
    {
        int n = i * chunkSize;

        std::vector<qreal> tmp(data.begin() + n - overlap, data.begin() + n + chunkSize);
        QVector<qreal> buf = QVector<qreal>::fromStdVector(tmp);

        fft(buf);
    }
    emit dataReady(m_out);
}
