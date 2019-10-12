#ifndef TRACK_H
#define TRACK_H

#include <QString>
#include <QStringList>
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
    int getFech()
    {
        return m_fech;
    }

    void setFech(int fech)
    {
        m_fech = fech;
    }

    QStringList getPaths()
    {
        return m_pathList;
    }

    void setPaths(QStringList& paths)
    {
        m_pathList = paths;
    }

    QString getName()
    {
        return m_name;
    }

    void setName(QString name)
    {
        m_name = name;
    }

    QString getDescription()
    {
        return m_description;
    }

    void setDescription(QString description)
    {
        m_description = description;
    }

    QString getCopyrights()
    {
        return m_copyrights;
    }

    void setCopyrights(QString copyrights)
    {
        m_copyrights = copyrights;
    }

private:

    static Track* m_trackInstance;


    QStringList m_pathList;
    QString m_name;
    QString m_description;
    QString m_copyrights;
    int m_fech;
};

#endif // TRACK_H
