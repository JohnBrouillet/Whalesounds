#ifndef UTILS_H
#define UTILS_H

#include <QAudioFormat>
#include <QVector>


qreal pcmToReal(qint16 pcm);
// get data of one channel
void getBufferLevels(const QByteArray & buffer, int channels, QVector<qreal>& out);

#endif // UTILS_H
