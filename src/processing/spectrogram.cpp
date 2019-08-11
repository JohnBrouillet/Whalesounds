#include "include/processing/spectrogram.h"
#include "include/audio/spectrum.h"

Spectrogram::Spectrogram() : m_HPFilter(HPF, 51, 44.1, 0.3)
{

}

void Spectrogram::computeFFT(int i, QVector<double> data)
{
    const int mean = 1;
    kiss_fft_cfg cfg = kiss_fft_alloc( NFFT , 0 , 0, 0 );
   // kiss_fft_cfg inverse_cfg = kiss_fft_alloc( NFFT , 1 , 0, 0 );

    QVector<double> out(int(maxFreqDisp * NFFT / fech), 0);

    for(int i = 0; i < mean; i++)
    {
        kiss_fft_cpx* cx_in = new kiss_fft_cpx[data.size() / mean];
        kiss_fft_cpx* cx_out = new kiss_fft_cpx[NFFT];
        //kiss_fft_cpx* cx_out_cepstre = new kiss_fft_cpx[NFFT];

        for(int j = i * data.size() / mean; j < (i+1) * data.size() / mean; j++)
        {
            cx_in[j - i * data.size() / mean].r = data[j] * 0.5 * (1 - std::cos((2 * M_PI * j) / (NFFT - 1)));
            cx_in[j - i * data.size() / mean].i = 0;
        }

        kiss_fft( cfg , cx_in , cx_out );

       /* for(int i = 0; i < NFFT; i++)
            cx_out[i].r = std::log(std::abs(cx_out[i].r));

        kiss_fft(inverse_cfg, cx_out, cx_out_cepstre);

        for(int i = 0; i < int(maxFreqDisp * NFFT / fech); i++)
            out[i] = std::abs(cx_out_cepstre[i].r);*/


        for(int d = 0; d < int(maxFreqDisp * NFFT / fech); d++)
        {
                out[d] += (std::abs(cx_out[d].r)) / mean;
        }

        delete[] cx_in;
        delete[] cx_out;
    }


    emit dataReady(out);
}

void Spectrogram::setFech(int _fech)
{
    fech = (double)_fech;
    m_HPFilter = Filter(HPF, 51, fech*0.001, 0.3);
}
