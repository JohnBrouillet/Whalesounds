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

public Q_SLOTS:
    void sendPaths(QString species);
    QVariantList getFamily(){ return famille; }
    QVariantList getEspeces(QString family);

Q_SIGNALS:
    void sendNames(QVariantList family, QVariantList species);

    void soundsPath(QStringList);
    void imagePath(QString _path);
    void name(QString _name);


private:
    QJsonDocument m_document;
    QJsonObject m_species;

    QMultiMap<QString, QString> m_speciesByFamily;
    QVariantList famille;
    QVariantList espece;

    QString absPath;
};

#endif // JSONCARETAKER_H
