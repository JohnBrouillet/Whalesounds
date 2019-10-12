#include "include/utils/jsoncaretaker.h"
#include "include/audio/track.h"


JsonCaretaker::JsonCaretaker(QString path, QString abspath)
{
    absPath = abspath;

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

        famille.push_back(family);
        foreach(const QString & _species, species.keys())
        {
            espece.push_back(_species);
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
    Q_EMIT imagePath(absPath + m_species[m_speciesByFamily.keys(species)[0]].toObject()[species].toObject()["image"].toString());
}

void JsonCaretaker::getSoundsPath(QString species)
{
    QVariantList soundsList = m_species[m_speciesByFamily.keys(species)[0]].toObject()[species].toObject()["sons"].toArray().toVariantList();

    QStringList sounds;
    foreach(const QVariant & s, soundsList)
        sounds << absPath + s.toString();

    Track::get()->setPaths(sounds);
}

void JsonCaretaker::getCopyrights(QString species)
{
    Track::get()->setCopyrights(m_species[m_speciesByFamily.keys(species)[0]].toObject()[species].toObject()["credits"].toString());
}

void JsonCaretaker::getDescription(QString species)
{
    Track::get()->setDescription(m_species[m_speciesByFamily.keys(species)[0]].toObject()[species].toObject()["description"].toString());
}

void JsonCaretaker::printKeyValues()
{
    qDebug() << m_speciesByFamily;
}

