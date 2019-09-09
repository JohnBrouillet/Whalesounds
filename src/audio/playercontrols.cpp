#include "include/audio/playercontrols.h"

#include <QBoxLayout>
#include <QSlider>
#include <QStyle>
#include <QToolButton>
#include <QComboBox>
#include <QAudio>

PlayerControls::PlayerControls(QWidget *parent)
    : QWidget(parent), m_engine(new Engine)
{
    indexFile = 0;
    isLoaded = false;
    engineConnect();

    QThread* decoderThread = new QThread;
    decoder.moveToThread(decoderThread);
    qRegisterMetaType<QVector<QPointF>>("QVector<double>");
    decoderThread->start();
}

void PlayerControls::engineConnect()
{
    connect(m_engine, &Engine::stateChanged, this, &PlayerControls::setState);
    connect(m_engine, &Engine::bufferChanged, &decoder, &BufferDecoderThread::getBuffer);
    connect(m_engine, &Engine::formatChanged, &decoder, &BufferDecoderThread::formatChanged);
    connect(&decoder, &BufferDecoderThread::bufferReady, this, &PlayerControls::bufferToGo);
    connect(m_engine, &Engine::dataLengthChanged, &decoder, &BufferDecoderThread::dataLengthChanged);
    connect(m_engine, &Engine::dataLengthChanged, this, &PlayerControls::dataLengthChanged);
    connect(m_engine, &Engine::channelCountChanged, this, &PlayerControls::channelCountChanged);
    connect(m_engine, &Engine::channelCountChanged, &decoder, &BufferDecoderThread::channelCountChanged);

    connect(m_engine, &Engine::playPositionChanged, this, &PlayerControls::updateDurationInfo);

    connect(m_engine, &Engine::errorMessage, [&](const QString& heading, const QString & detail){
        qDebug() << "heading " << heading;
        qDebug() << "detail " << detail;
    });

    connect(this, &PlayerControls::generatorBuffer, m_engine, &Engine::setGeneratorBuffer);
}



void PlayerControls::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    qint64 time = audioDuration(m_engine->format(), currentInfo)*1e-6;
    if (currentInfo || m_duration)
    {
        QTime currentTime((time / 3600) % 60, (time / 60) % 60, time % 60, (time * 1000) % 1000);
        QTime totalTime((m_duration / 3600) % 60, (m_duration / 60) % 60, m_duration % 60, (m_duration * 1000) % 1000);
        QString format = "mm:ss";
        if (m_duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    Q_EMIT newTime(tStr);
}


void PlayerControls::loadFiles(QStringList filenames)
{
   indexFile = 0;
   isLoaded = false;

   filesPath.clear();
   foreach(const QString & file, filenames)
       filesPath.push_back(file);

   Q_EMIT newIndexFile(indexFile, filesPath.size());
   Q_EMIT newFiles();
}

QAudio::State PlayerControls::state() const
{
    return m_playerState;
}

void PlayerControls::setState(QAudio::Mode mode, QAudio::State state)
{
    Q_UNUSED(mode);
    if (state != m_playerState) {
        m_playerState = state;
        if(m_playerState == QAudio::State::ActiveState)
            Q_EMIT newState(0);
        else if(m_playerState == QAudio::State::SuspendedState)
            Q_EMIT newState(1);
        else if( m_playerState == QAudio::State::StoppedState )
        {
            Q_EMIT newState(1);
            isLoaded = false;
        }
        else
            Q_EMIT newState(1);
    }
}


void PlayerControls::playClicked()
{
    if(filesPath.size())
    {
        switch (m_playerState)
        {
            case QAudio::State::StoppedState:
            case QAudio::State::SuspendedState:
            {
                if(!isLoaded)
                {
                    m_engine->reset();
                    if(m_engine->loadFile(filesPath[indexFile]))
                    {
                        m_duration = audioDuration(m_engine->format(), m_engine->bufferLength())*1e-6;
                        updateDurationInfo(m_duration);
                        Q_EMIT newFech(m_engine->format().sampleRate());

                        m_engine->startPlayback();
                        isLoaded = true;
                    }
                    else
                    {
                        Q_EMIT newText("Fichier introuvable");
                        Q_EMIT newState(1);
                    }
                    break;
                }
                Q_EMIT newState(0);

                m_engine->startPlayback();

            }
                break;
            case QAudio::State::ActiveState:
            {
                m_engine->suspend();
                Q_EMIT newState(1);
            }
                break;
        }
    }

}

void PlayerControls::previous()
{
    if(indexFile == 0)
        indexFile = 0;
    else
        indexFile--;

    isLoaded = false;
    Q_EMIT newIndexFile(indexFile, filesPath.size());

    if(filesPath.size() != 0)
    {
        playClicked();
        playClicked();
    }
}

void PlayerControls::next()
{
    if(indexFile == filesPath.size() - 1)
        indexFile = filesPath.size() - 1;
    else
        indexFile++;

    isLoaded = false;
    Q_EMIT newIndexFile(indexFile, filesPath.size());

    if(filesPath.size() != 0)
    {
        playClicked();
        playClicked();
    }
}
