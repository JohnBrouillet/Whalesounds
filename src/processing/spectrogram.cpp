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

void Spectrogram::fft(QVector<double>& data, bool padding)
{
    int nbData = data.size();
    if(padding)
    {
        for(int i = 0; i < NFFT - nbData; i++)
            data << 0.0;
    }

    double fft[NFFT];

    m_transformer.forwardTransform(data.data(), fft);
    QComplexVector complexVec = m_transformer.toComplex(fft);

    for(int i = 0; i < complexVec.size()-1; i++)
    {
        double real = complexVec[i].real();
        double imag = complexVec[i].imaginary();

        m_out.push_back(20*std::log((real*real + imag*imag)*1e6));
    }
}

void Spectrogram::computeFFT()
{
    m_out.clear();
    std::vector<qreal> data = Track::get()->getData().toStdVector();
    double overlapPerc = 0.5;

    int chunkSize = temporalResolution * Track::get()->getFormat().sampleRate();
    int overlap = chunkSize * overlapPerc;

    std::vector<qreal> tmp(data.begin(), data.begin() + chunkSize);
    QVector<qreal> buf = QVector<qreal>::fromStdVector(tmp);

    fft(buf, true);

    for(int i = 1; i < (data.size() / chunkSize); i++)
    {
      int n = i * chunkSize;

      std::vector<qreal> tmp(data.begin() + n - overlap, data.begin() + n + chunkSize);
      qreal moy = std::accumulate(tmp.begin(), tmp.end(), 0.0) / tmp.size();
      std::transform(tmp.begin(), tmp.end(), tmp.begin(), [&](qreal i){ return i - moy;});
      QVector<qreal> buf = QVector<qreal>::fromStdVector(tmp);

      fft(buf,true);
    }
    emit dataReady(m_out);
}
