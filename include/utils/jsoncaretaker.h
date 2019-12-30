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
#include <QQmlEngine>
#include <iostream>


class JsonCaretaker : public QObject
{
    Q_OBJECT

public:
    JsonCaretaker(){}
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
    QVariantList getAllImagesPaths();
    QVariantList getAllImagesPathsFamily(QString family);

Q_SIGNALS:
    void sendNames(QVariantList family, QVariantList species);

    void soundsPath(QStringList _paths);
    void imagePath(QString _path);
    void name(QString _name);
    void sendCopyrights(QString _copyrights);
    void sendDescription(QString _description);

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
