#include "include/utils/utils.h"

void getBufferLevels(const QByteArray & buffer, int channels, QVector<qreal>& out)
{
    const qint16* data = reinterpret_cast<const qint16*>(buffer.constData());

    // le buffer contient des éléments sur 8 octets. En les castant sur 16 octets, son nombre de cases est divisé par 2.
    for (int i = 0; i < buffer.size() / (2*channels); i++, data += channels)
        out.push_back(pcmToReal(*data));
}

const quint16 PCMS16MaxAmplitude =  32768;

qreal pcmToReal(qint16 pcm)
{
    return qreal(pcm) / PCMS16MaxAmplitude;
}
