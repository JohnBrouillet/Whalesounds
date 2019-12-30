#include "include/audio/track.h"
#include "include/utils/utils.h"
#include <iostream>
Track *Track::m_trackInstance = nullptr;

QStringList Track::getPaths()
{
    return m_pathList;
}

void Track::setPaths(QStringList& paths)
{
    m_pathList = paths;
}

QString Track::getName()
{
    Q_EMIT name(m_name);
    return m_name;
}

void Track::setName(QString name)
{
    m_name = name;
}

QString Track::getDescription()
{
    return m_description;
}

void Track::setDescription(QString description)
{
    m_description = description;
}

QString Track::getCopyrights()
{
    return m_copyrights;
}

void Track::setCopyrights(QString copyrights)
{
    m_copyrights = copyrights;
}

QVector<qreal> Track::getData()
{
    return m_data;
}

void Track::setData(QByteArray& data)
{
    QVector<qreal> tmp(m_format.channelCount());
    getBufferLevels(data, m_format.channelCount(), tmp);
    m_data = tmp;
}

void Track::setFormat(QAudioFormat format)
{
    m_format = format;
}

QAudioFormat Track::getFormat()
{
    return m_format;
}
