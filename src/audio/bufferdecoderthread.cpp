#include "include/audio/bufferdecoderthread.h"
#include "include/utils/utils.h"
#define MSG(text) std::cout << text << std::endl;

void getBufferLevels(const QByteArray & buffer, int channels, qreal peak, QVector<QVector<qreal>>& datas)
{
    const qint16* data = reinterpret_cast<const qint16*>(buffer.constData());

    // le buffer contient des éléments sur 8 octets. En les castant sur 16 octets, son nombre de cases est divisé par 2.
    for (int i = 0; i < buffer.size() / (2*channels); i++, data += channels)
    {
        datas[0].push_back(pcmToReal(*data));
    }

}



qreal getPeakValue(const QAudioFormat& format)
{
    // Note: Only the most common sample formats are supported
    if (!format.isValid())
        return qreal(0);

    if (format.codec() != "audio/pcm")
        return qreal(0);

    switch (format.sampleType()) {
    case QAudioFormat::Unknown:
        break;
    case QAudioFormat::Float:
        if (format.sampleSize() != 32) // other sample formats are not supported
            return qreal(0);
        return qreal(1.00003);
    case QAudioFormat::SignedInt:
        if (format.sampleSize() == 32)
            return qreal(INT_MAX);
        if (format.sampleSize() == 16)
            return qreal(SHRT_MAX);
        if (format.sampleSize() == 8)
            return qreal(CHAR_MAX);
        break;
    case QAudioFormat::UnSignedInt:
        if (format.sampleSize() == 32)
            return qreal(UINT_MAX);
        if (format.sampleSize() == 16)
            return qreal(USHRT_MAX);
        if (format.sampleSize() == 8)
            return qreal(UCHAR_MAX);
        break;
    }

    return qreal(0);
}

void getBufferLevels(const QAudioFormat & format, const QByteArray & buffer, QVector<QVector<qreal>>& values)
{

    int channelCount = format.channelCount();

    qreal peak = getPeakValue(format);

    switch (format.sampleType())
    {
        case QAudioFormat::Unknown:
        case QAudioFormat::UnSignedInt:
            if (format.sampleSize() == 32)
                getBufferLevels(buffer, channelCount, peak, values);
            if (format.sampleSize() == 16)
                getBufferLevels(buffer, channelCount, peak, values);
            if (format.sampleSize() == 8)
                getBufferLevels(buffer, channelCount, peak, values);
            /*for (int i = 0; i < values.size(); ++i)
                values[i] = qAbs(values.at(i) - peak_value / 2) / (peak_value / 2);*/
            break;
        case QAudioFormat::Float:
            if (format.sampleSize() == 32) {
                getBufferLevels(buffer, channelCount, peak, values);
               /* for (int i = 0; i < values.size(); ++i)
                    values[i] /= peak_value;*/
            }
            break;
        case QAudioFormat::SignedInt:
            if (format.sampleSize() == 32)
                getBufferLevels(buffer, channelCount, peak, values);
            if (format.sampleSize() == 16)
                getBufferLevels(buffer, channelCount, peak, values);
            if (format.sampleSize() == 8)
                getBufferLevels(buffer, channelCount, peak, values);
           /* for (int i = 0; i < values.size(); ++i)
                values[i] /= peak_value;*/
            break;
    }

}

void BufferDecoderThread::formatChanged(const QAudioFormat & _format)
{
    format = _format;
}

void BufferDecoderThread::dataLengthChanged(qint64 value)
{
    sampleCount = value / ( 2 * channelCount);
}

void BufferDecoderThread::channelCountChanged(int value)
{
    channelCount = value;
}

void BufferDecoderThread::getBuffer(qint64 position, qint64 length, const QByteArray &buffer)
{
    const int buf_size = length / (2 * format.channelCount());
    if(!init)
    {
        m_buffer.resize(1);
        for(int i = 0; i < 1; i++)
        {
            m_buffer[i].reserve(sampleCount);
            for (int j = 0; j < sampleCount; ++j)
            {
                m_buffer[i].append(0);
            }
        }

        init = true;
    }


    QVector<QVector<qreal>> new_data(1);
    getBufferLevels(format, buffer, new_data);
    static const int resolution = 1;
    int availableSamples = buf_size;
    for(int i = 0; i < 1; i++)
    {
        /*if (availableSamples < sampleCount)
        {
            QVector<double>::const_iterator first = m_buffer[i].begin() + availableSamples;
            QVector<double>::const_iterator last = m_buffer[i].end();

            std::vector<double> new_buffer(first, last);
            new_buffer.insert(new_buffer.end(), new_data[i].begin(), new_data[i].end());

            m_buffer[i] = new_data[i]; //QVector<double>::fromStdVector(new_buffer);

        }*/
        m_buffer[i] = new_data[i];
       emit bufferReady(i, m_buffer[i]);
    }

}
