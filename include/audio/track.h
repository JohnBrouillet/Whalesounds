#ifndef TRACK_H
#define TRACK_H

#include <QString>
#include <QStringList>
#include <QByteArray>
#include <QAudioFormat>
#include <QObject>

class Track : public QObject
{
    Q_OBJECT

public:
    static Track *get()
    {
        if(!m_trackInstance)
            m_trackInstance = new Track;

        return m_trackInstance;
    }

    static void destroy()
    {
        delete m_trackInstance;
        m_trackInstance = nullptr;
    }

public Q_SLOTS:
    QStringList getPaths();
    void setPaths(QStringList& paths);
    QString getName();
    void setName(QString name);
    QString getDescription();
    void setDescription(QString description);
    QString getCopyrights();
    void setCopyrights(QString copyrights);
    QVector<QVector<qreal>> getData();
    void setData(QByteArray& data);
    QAudioFormat getFormat();
    void setFormat(QAudioFormat format);

private:
    static Track* m_trackInstance;

    QVector<QVector<qreal>> m_data;
    QAudioFormat m_format;
    QStringList m_pathList;
    QString m_name;
    QString m_description;
    QString m_copyrights;
};

#endif // TRACK_H
