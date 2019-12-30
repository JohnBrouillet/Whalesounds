#include "include/utils/jsoncaretaker.h"
#include "include/audio/track.h"

JsonCaretaker::JsonCaretaker(QString path, QString abspath)
{
    m_absPath = abspath;

    QFile file;
    file.setFileName(path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QByteArray val = file.readAll();
    file.close();

    m_document = QJsonDocument::fromJson(val);
    m_species = m_document.object();

}

void JsonCaretaker::setSpecies()
{
    foreach(const QString & family, m_species.keys())
    {
        QJsonObject species = m_species.value(family).toObject();

        m_famille.push_back(family);
        foreach(const QString & _species, species.keys())
        {
            m_espece.push_back(_species);
            m_speciesByFamily.insert(family, _species);
        }
    }
}

QVariantList JsonCaretaker::getEspeces(QString family)
{
    QVariantList species;
    QStringList listSpecies = m_speciesByFamily.values(family);

    foreach(const QString & specie, listSpecies)
        species << specie << m_species[m_speciesByFamily.keys(specie)[0]].toObject()[specie].toObject()["sons"].toArray().toVariantList().size();

    return species;
}

QVariantList JsonCaretaker::getAllImagesPaths()
{
    QVariantList paths;

    foreach(const QVariant & family, m_famille)
        paths << getAllImagesPathsFamily(family.toString());

    return paths;
}

QVariantList JsonCaretaker::getAllImagesPathsFamily(QString family)
{
    QVariantList paths;

    QStringList listSpecies = m_speciesByFamily.values(family);
    foreach(const QString & specie, listSpecies)
        paths << specie << (m_absPath + m_species[m_speciesByFamily.keys(specie)[0]].toObject()[specie].toObject()["image"].toString());

    return paths;
}

void JsonCaretaker::sendPaths(QString species)
{
    Track::get()->setName(species);

    getImagePath(species);
    getSoundsPath(species);
    getCopyrights(species);
    getDescription(species);

    Q_EMIT newTrack();
}

void JsonCaretaker::getImagePath(QString species)
{
    Q_EMIT imagePath(m_absPath + m_species[m_speciesByFamily.keys(species)[0]].toObject()[species].toObject()["image"].toString());
}

void JsonCaretaker::getSoundsPath(QString species)
{
    QVariantList soundsList = m_species[m_speciesByFamily.keys(species)[0]].toObject()[species].toObject()["sons"].toArray().toVariantList();

    QStringList sounds;
    foreach(const QVariant & s, soundsList)
        sounds << m_absPath + s.toString();

    Q_EMIT soundsPath(sounds);
    Track::get()->setPaths(sounds);
}

void JsonCaretaker::getCopyrights(QString species)
{
    QString copyrights = m_species[m_speciesByFamily.keys(species)[0]].toObject()[species].toObject()["credits"].toString();

    Q_EMIT sendCopyrights(copyrights);
    Track::get()->setCopyrights(copyrights);
}

void JsonCaretaker::getDescription(QString species)
{
    QString description(m_species[m_speciesByFamily.keys(species)[0]].toObject()[species].toObject()["description"].toString());
    Q_EMIT sendDescription(description);
    Track::get()->setDescription(description);
}

void JsonCaretaker::printKeyValues()
{
    qDebug() << m_speciesByFamily;
}

