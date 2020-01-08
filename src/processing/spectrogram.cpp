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
    double overlapPerc = 0.0;

    int resolutionSize = int(temporalResolution * double(Track::get()->getFormat().sampleRate()));
    int chunkSize = NFFT;
    int overlap = (chunkSize - resolutionSize) / 2;


    m_out.reserve((data.size() / chunkSize)*NFFT/2);


    for(int i = 0; i < (data.size() / resolutionSize) - 1; i++)
    {
        int n = i * resolutionSize;
        QVector<qreal> buf;

        if(((n + resolutionSize + overlap) < data.size()) && n > chunkSize)
        {
            std::vector<qreal> tmp(data.begin() + n - overlap-1, data.begin() + n + resolutionSize + overlap);
            buf = QVector<qreal>::fromStdVector(tmp);
            fft(buf, false);
        }
        else if(n < chunkSize)
        {
            std::vector<qreal> tmp(data.begin() + n , data.begin() + n + resolutionSize);
            buf = QVector<qreal>::fromStdVector(tmp);
            fft(buf, true);

        }
        else
        {
            std::vector<qreal> tmp(data.begin() + n, data.end());
            buf = QVector<qreal>::fromStdVector(tmp);
            fft(buf, true);
        }
    }
    emit dataReady(m_out);
}
