#ifndef PLAYERCONTROLS_H
#define PLAYERCONTROLS_H

#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QWidget>
#include <QAudio>
#include <QTime>
#include <QFileInfo>

class PlayerControls : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerControls(QWidget *parent = nullptr);

    QMediaPlayer::State state() const;
    qreal playbackRate() const;

public Q_SLOTS:
    void setState(QMediaPlayer::State state);
    void loadFiles();
    void updateDurationInfo(qint64 currentInfo);
    void durationChanged(qint64 duration);
    void next();
    void previous();
    void playClicked();
    void changeRate(int value);
    void seek(int seconds);
    void playlistPositionChanged(int position);
    void reloadBuffer(bool reload)
    {
        m_isLoaded = reload;
        m_player->stop();
    }

Q_SIGNALS:
    void newTrack();
    void newFiles();
    void newTime(QString time);
    void newText(QString _text);
    void newIndexFile(int indexFile, int nbfiles);
    void newState(bool state);
    void newPosition(double value);
    void newDuration(qint64 value);


private:
    enum STATE {PLAYING, STOP};
    double m_rate[8] = {0.25, 0.5, 0.75, 1.0, 1.25, 1.5, 1.75, 2.0};

    QMediaPlaylist * m_playlist;
    QMediaPlayer * m_player;
    QMediaPlayer::State m_playerState = QMediaPlayer::StoppedState;

    qint64 m_duration;
    bool m_isLoaded;
};

#endif // PLAYERCONTROLS_H
