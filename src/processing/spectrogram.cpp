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

void Spectrogram::fft(QVector<double>& data)
{

    int nbData = data.size();
    if(nbData < NFFT)
    {
        for(int i = 0; i < NFFT - nbData; i++)
            data << 0.0;
    }

    double fft[NFFT];

    m_transformer.forwardTransform(data.data(), fft);
    QComplexVector complexVec = m_transformer.toComplex(fft);


    for(int i = 0; i < complexVec.size()-1; i++)
    {
        /*double tmp = std::log(std::abs(fft[i]));
        tmp = tmp >= 0 ? tmp : 0;
        m_out.push_back(20*std::abs(tmp));*/

        double real = complexVec[i].real();
        double imag = complexVec[i].imaginary();

        m_out.push_back(10*std::log(real*real + imag*imag)*1e6);
    }
}

void Spectrogram::computeFFT()
{
    m_out.clear();

    std::vector<qreal> data = Track::get()->getData().toStdVector();
    double overlapPerc = 0.5;

    int chunkSize = int(temporalResolution * double(Track::get()->getFormat().sampleRate()));
    int overlap = chunkSize * overlapPerc;

    std::vector<qreal> tmp(data.begin(), data.begin() + chunkSize);
    QVector<qreal> buf = QVector<qreal>::fromStdVector(tmp);

    fft(buf);

    for(int i = 1; i < (data.size() / chunkSize) - 1; i++)
    {
        int n = i * chunkSize;

        std::vector<qreal> tmp(data.begin() + n - overlap, data.begin() + n + chunkSize);
        QVector<qreal> buf = QVector<qreal>::fromStdVector(tmp);

        fft(buf);
    }
    emit dataReady(m_out);
}
