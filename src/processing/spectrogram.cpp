#include "include/processing/spectrogram.h"
#include "include/audio/spectrum.h"

Spectrogram::Spectrogram() : m_HPFilter(HPF, 51, 44.1, 0.3)
{

}

void Spectrogram::computeFFT(int channel, QVector<double> data)
{
    const int mean = 1;
    kiss_fft_cfg cfg = kiss_fft_alloc(NFFT, 0, 0, 0);
    QVector<double> out(int(maxFreqDisp * NFFT / fech), 0);

    int chunkSize = data.size() / mean;

    kiss_fft_cpx* cx_in = new kiss_fft_cpx[chunkSize];
    kiss_fft_cpx* cx_out = new kiss_fft_cpx[NFFT];

    for(int i = 0; i < mean; i++)
    {
        for(int j = i * chunkSize; j < (i+1) * chunkSize; j++)
        {
            int index = j - i * chunkSize;
            cx_in[index].r = data[j] * 0.5 * (1 - std::cos((2 * M_PI * index) / (NFFT - 1)));
            cx_in[index].i = 0;
        }

        kiss_fft( cfg , cx_in , cx_out );

        for(int d = 0; d < int(maxFreqDisp * NFFT / fech); d++)
                out[d] += (std::abs(cx_out[d].r)) / mean;
    }

    delete[] cx_in;
    delete[] cx_out;
    emit dataReady(out);
}

void Spectrogram::setFech(int _fech)
{
    fech = (double)_fech;
    m_HPFilter = Filter(HPF, 51, fech*0.001, 0.3);
}
