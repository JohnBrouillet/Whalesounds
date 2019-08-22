#ifndef BUFFERDECODERTHREAD_H
#define BUFFERDECODERTHREAD_H

#include <QObject>
#include <QAudioFormat>
#include <QPointF>
#include <iostream>
#include <QDebug>

class BufferDecoderThread : public QObject
{
    Q_OBJECT

public:
    BufferDecoderThread()
    {
        channelCount = 2;
        sampleCount = 200000;
        init = false;
    }


public Q_SLOTS:
    void getBuffer(qint64 position, qint64 length, const QByteArray &buffer);
    void formatChanged(const QAudioFormat &format);
    void dataLengthChanged(qint64 value);
    void channelCountChanged(int value);
Q_SIGNALS:
    void bufferReady(int index, QVector<double>);

private:
    QVector<QVector<double>> m_buffer;
    QAudioFormat format;

    int sampleCount;
    int channelCount;
    bool init;
};

#endif // BUFFERDECODERTHREAD_H
