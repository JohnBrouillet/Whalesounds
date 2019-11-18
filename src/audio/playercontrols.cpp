#include "include/audio/playercontrols.h"
#include "include/audio/track.h"
#include "include/audio/wavfile.h"

PlayerControls::PlayerControls(QWidget *parent)
    : QWidget(parent)
{
    m_isLoaded = false;

    m_player = new QMediaPlayer(this);
    m_player->setAudioRole(QAudio::MusicRole);
    m_playlist = new QMediaPlaylist;
    m_player->setPlaylist(m_playlist);

#ifdef ANDROID
    m_player->setNotifyInterval(100);
#else
    m_player->setNotifyInterval(10);
#endif

    connect(m_player, &QMediaPlayer::durationChanged, this, &PlayerControls::durationChanged);
    connect(m_player, &QMediaPlayer::positionChanged, this, &PlayerControls::updateDurationInfo);
    connect(m_player, &QMediaPlayer::stateChanged, this, &PlayerControls::setState);
    connect(m_playlist, &QMediaPlaylist::currentIndexChanged, this, &PlayerControls::playlistPositionChanged);
}

void PlayerControls::durationChanged(qint64 duration)
{
    m_duration = duration / 1000;
    Q_EMIT newDuration(m_duration);
}

void PlayerControls::updateDurationInfo(qint64 currentInfo)
{
    QString timeString;
    qint64 time = currentInfo / 1000;
    if (currentInfo || m_duration)
    {
        QTime currentTime((time / 3600) % 60, (time / 60) % 60, time % 60, (time * 1000) % 1000);
        QTime totalTime((m_duration / 3600) % 60, (m_duration / 60) % 60, m_duration % 60, (m_duration * 1000) % 1000);
        QString format = "mm:ss";
        if (m_duration > 3600)
            format = "hh:mm:ss";
        timeString = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    else
        timeString = "00/00";
    Q_EMIT newTime(timeString);
    Q_EMIT newPosition(currentInfo / 1000.0);
}

static bool isPlaylist(const QUrl &url)
{
    if (!url.isLocalFile())
        return false;
    const QFileInfo fileInfo(url.toLocalFile());
    return fileInfo.exists() && !fileInfo.suffix().compare(QLatin1String("m3u"), Qt::CaseInsensitive);
}

void PlayerControls::loadFiles()
{
   m_isLoaded = false;

   m_playlist->clear();
   foreach(const QString & file, Track::get()->getPaths())
   {
#if defined(ANDROID) || defined(__unix__)
       QUrl url = QUrl("file://"+file);
#else
       QUrl url = QUrl(file);
#endif
       if (isPlaylist(url))
           m_playlist->load(url);
       else
           m_playlist->addMedia(url);
    }

   Q_EMIT newIndexFile(0, m_playlist->mediaCount());
   Q_EMIT newFiles();
}

QMediaPlayer::State PlayerControls::state() const
{
    return m_playerState;
}

void PlayerControls::setState(QMediaPlayer::State state)
{
    if (state != m_playerState) {
        m_playerState = state;
        if(m_playerState ==  QMediaPlayer::PlayingState)
            Q_EMIT newState(PLAYING);
        else if(m_playerState ==  QMediaPlayer::PausedState)
            Q_EMIT newState(STOP);
        else if( m_playerState == QMediaPlayer::StoppedState )
        {
            Q_EMIT newState(STOP);
            m_isLoaded = false;
        }
        else
            Q_EMIT newState(STOP);
    }
}


void PlayerControls::playClicked()
{
    if(m_playlist->mediaCount())
    {
        switch (m_playerState)
        {
            case QMediaPlayer::StoppedState:
            case QMediaPlayer::PausedState:
            {
                if(!m_isLoaded)
                {
                    m_player->play();
                    m_isLoaded = true;
                    break;
                }
                Q_EMIT newState(PLAYING);
                m_player->play();
            }
                break;
            case QMediaPlayer::PlayingState:
            {
                m_player->pause();
                Q_EMIT newState(STOP);
            }
                break;
        }
    }
}

void PlayerControls::previous()
{
    m_playlist->previous();
}

void PlayerControls::next()
{
    m_playlist->next();
}

void PlayerControls::seek(int seconds)
{
    m_player->setPosition(seconds * 1000);
}

void PlayerControls::changeRate(int value)
{
    m_player->setPlaybackRate(m_rate[value]);
}

void PlayerControls::playlistPositionChanged(int position)
{
    if(position != -1)
    {
        WavFile m_file;

        QString path = m_playlist->currentMedia().canonicalUrl().toString();
#if defined(ANDROID) || defined(__unix__)
        path.remove(0, 7);
#endif

        if(m_file.open(path))
        {
            m_file.seek(m_file.headerLength());
            qint64 fileSize = m_file.size() - m_file.headerLength();

            QByteArray tmp;
            tmp.resize(fileSize);
            m_file.read(tmp.data(), fileSize);
            Track::get()->setFormat(m_file.fileFormat());
            Track::get()->setData(tmp);
            m_file.close();

            Q_EMIT newIndexFile(position, m_playlist->mediaCount());

            Q_EMIT newTrack();
        }
    }
}
