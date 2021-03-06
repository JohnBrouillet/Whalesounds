#ifndef JSONCARETAKER_H
#define JSONCARETAKER_H

#include <QObject>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QVariantList>
#include <QMultiMap>
#include <QDebug>
#include <iostream>


class JsonCaretaker : public QObject
{
    Q_OBJECT

public:
    JsonCaretaker(QString path, QString abspath);
    void setSpecies();
    void printKeyValues();

    void getImagePath(QString species);
    void getSoundsPath(QString species);
    void getCopyrights(QString species);
    void getDescription(QString species);

public Q_SLOTS:
    void sendPaths(QString species);
    QVariantList getFamily(){ return m_famille; }
    QVariantList getEspeces(QString family);

Q_SIGNALS:
    void sendNames(QVariantList family, QVariantList species);

    void soundsPath(QString, QStringList);
    void imagePath(QString _path);
    void name(QString _name);
    void copyrights(QString _copyrights);

    void newTrack();


private:
    QJsonDocument m_document;
    QJsonObject m_species;

    QMultiMap<QString, QString> m_speciesByFamily;
    QVariantList m_famille;
    QVariantList m_espece;

    QString m_absPath;
};

#endif // JSONCARETAKER_H
