#include "include/processing/spectrogram.h"
#include "include/audio/spectrum.h"
#include "include/audio/track.h"



Spectrogram::Spectrogram() : m_HPFilter(HPF, 51, 44.1, 0.3)
{
    cx_out = new kiss_fft_cpx[NFFT];
    cfg = kiss_fft_alloc(NFFT, 0, 0, 0);
}

Spectrogram::~Spectrogram()
{
    delete[] cx_out;
}

void Spectrogram::computeFFT(int channel, QVector<double> data)
{
    QVector<double> out(nbOutputData, 0);

    int chunkSize = data.size();

    kiss_fft_cpx* cx_in = new kiss_fft_cpx[chunkSize];

    for(int i = 0; i < chunkSize; i++)
    {
        cx_in[i].r = data[i] * 0.5 * (1 - std::cos((2 * M_PI * i) / (NFFT - 1)));
        cx_in[i].i = 0;       
    }

    kiss_fft( cfg , cx_in , cx_out );

    for(int d = 0; d < nbOutputData; d++)
            out[d] += (std::abs(cx_out[d].r));

    delete[] cx_in;
    emit dataReady(out);
}

void Spectrogram::setFech()
{
    fech = double(Track::get()->getFech());
    nbOutputData = int(maxFreqDisp * NFFT / fech);
    m_HPFilter = Filter(HPF, 51, fech*0.001, 0.3);
}
