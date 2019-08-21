/****************************************************************************
**
** Copyright (C) 2017 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of the examples of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:BSD$
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** BSD License Usage
** Alternatively, you may use this file under the terms of the BSD license
** as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of The Qt Company Ltd nor the names of its
**     contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#include "include/audio/engine.h"
#include "include/audio/tonegenerator.h"
#include "include/utils/utils.h"

#include <math.h>

#include <QAudioInput>
#include <QAudioOutput>
#include <QCoreApplication>
#include <QDebug>
#include <QFile>
#include <QMetaObject>
#include <QSet>
#include <QThread>

//-----------------------------------------------------------------------------
// Constants
//-----------------------------------------------------------------------------

const qint64 BufferDurationUs       = 15000000;
const int    NotifyIntervalMs       = 100;

// Size of the level calculation window in microseconds
const int    LevelWindowUs          = 0.1 * 1000000;

//-----------------------------------------------------------------------------
// Constructor and destructor
//-----------------------------------------------------------------------------

Engine::Engine(QObject *parent)
    :   QObject(parent)
    ,   m_mode(QAudio::AudioInput)
    ,   m_state(QAudio::StoppedState)
    ,   m_file(0)
    ,   m_analysisFile(0)
    ,   m_availableAudioInputDevices
            (QAudioDeviceInfo::availableDevices(QAudio::AudioInput))
    ,   m_audioInputDevice(QAudioDeviceInfo::defaultInputDevice())
    ,   m_audioInput(0)
    ,   m_audioInputIODevice(0)
    ,   m_recordPosition(0)
    ,   m_availableAudioOutputDevices
            (QAudioDeviceInfo::availableDevices(QAudio::AudioOutput))
    ,   m_audioOutputDevice(QAudioDeviceInfo::defaultOutputDevice())
    ,   m_audioOutput(0)
    ,   m_playPosition(0)
    ,   m_bufferPosition(0)
    ,   m_bufferLength(0)
    ,   m_dataLength(0)
    ,   m_prevDataLength(0)
    ,   m_levelBufferLength(0)
    ,   m_rmsLevel(0.0)
    ,   m_peakLevel(0.0)
    ,   m_spectrumBufferLength(0)
   // ,   m_spectrumAnalyser()
    ,   m_spectrumPosition(0)
    ,   m_count(0)
{
    //qRegisterMetaType<FrequencySpectrum>("FrequencySpectrum");
   // qRegisterMetaType<WindowFunction>("WindowFunction");
    //connect(&m_spectrumAnalyser, QOverload<const FrequencySpectrum&>::of(&SpectrumAnalyser::spectrumChanged),
     //       this, QOverload<const FrequencySpectrum&>::of(&Engine::spectrumChanged));

    // This code might misinterpret things like "-something -category".  But
    // it's unlikely that that needs to be supported so we'll let it go.
    QStringList arguments = QCoreApplication::instance()->arguments();
    for (int i = 0; i < arguments.count(); ++i) {
        if (arguments.at(i) == QStringLiteral("--"))
            break;

        if (arguments.at(i) == QStringLiteral("-category")
                || arguments.at(i) == QStringLiteral("--category")) {
            ++i;
            if (i < arguments.count())
                m_audioOutputCategory = arguments.at(i);
            else
                --i;
        }
    }
    m_audioOutputCategory = "media";
    initialize();
}

Engine::~Engine()
{

}

//-----------------------------------------------------------------------------
// Public functions
//-----------------------------------------------------------------------------

bool Engine::loadFile(const QString &fileName)
{
    reset();
    bool result = false;
    Q_ASSERT(!m_file);
    Q_ASSERT(!fileName.isEmpty());
    m_file = new WavFile(this);

    if (m_file->open(fileName))
    {
        if (isPCMS16LE(m_file->fileFormat()))
            result = initialize();
        else
            Q_EMIT errorMessage(tr("Audio format not supported"), formatToString(m_file->fileFormat()));
    }
    else
        Q_EMIT errorMessage(tr("Could not open file"), fileName);

    if (result)
    {
        m_analysisFile = new WavFile(this);
        m_analysisFile->open(fileName);
    }

    return result;
}


qint64 Engine::bufferLength() const
{
    return m_file ? m_file->size() : m_bufferLength;
}




//-----------------------------------------------------------------------------
// Public slots
//-----------------------------------------------------------------------------


void Engine::startPlayback()
{
    if (m_audioOutput)
    {
        if (QAudio::AudioOutput == m_mode &&
            QAudio::SuspendedState == m_state)
        {
#ifdef Q_OS_WIN
            // The Windows backend seems to internally go back into ActiveState
            // while still returning SuspendedState, so to ensure that it doesn't
            // ignore the resume() call, we first re-suspend
            m_audioOutput->suspend();
#endif
            m_audioOutput->resume();
        }
        else
        {
            setPlayPosition(0, true);
            m_mode = QAudio::AudioOutput;

            connect(m_audioOutput, &QAudioOutput::stateChanged, this, &Engine::audioStateChanged);
            connect(m_audioOutput, &QAudioOutput::notify, this, &Engine::audioNotify);

            m_count = 0;
            if (m_file)
            {
                m_file->seek(0);
                m_bufferPosition = 0;
                m_dataLength = 0;
                m_audioOutput->start(m_file);
            }
            else
            {
                m_audioOutputIODevice.close();
                m_audioOutputIODevice.setBuffer(&m_buffer);
                m_audioOutputIODevice.open(QIODevice::ReadOnly);
                m_audioOutput->start(&m_audioOutputIODevice);
            }
        }
    }
}

void Engine::suspend()
{
    if (QAudio::ActiveState == m_state ||
        QAudio::IdleState == m_state)
    {
        switch (m_mode)
        {
            case QAudio::AudioInput:
                m_audioInput->suspend();
                break;
            case QAudio::AudioOutput:
                m_audioOutput->suspend();
                break;
        }
    }
}

void Engine::setAudioInputDevice(const QAudioDeviceInfo &device)
{
    if (device.deviceName() != m_audioInputDevice.deviceName())
    {
        m_audioInputDevice = device;
        initialize();
    }
}

void Engine::setAudioOutputDevice(const QAudioDeviceInfo &device)
{
    if (device.deviceName() != m_audioOutputDevice.deviceName())
    {
        m_audioOutputDevice = device;
        initialize();
    }
}


//-----------------------------------------------------------------------------
// Private slots
//-----------------------------------------------------------------------------

void Engine::audioNotify()
{
    switch (m_mode)
    {
        case QAudio::AudioInput:
        {
                const qint64 recordPosition = qMin(m_bufferLength, audioLength(m_format, m_audioInput->processedUSecs()));
                const qint64 levelPosition = m_dataLength - m_levelBufferLength;
                Q_EMIT bufferChanged(0, m_dataLength, m_buffer);
        }
        break;
        case QAudio::AudioOutput:
        {
            const qint64 playPosition = audioLength(m_format, m_audioOutput->processedUSecs());
            setPlayPosition(qMin(bufferLength(), playPosition));
            const qint64 levelPosition = playPosition - m_levelBufferLength;
            const qint64 spectrumPosition = playPosition - m_spectrumBufferLength;
            if (m_file)
            {
                if (levelPosition > m_bufferPosition || spectrumPosition > m_bufferPosition ||
                    qMax(m_levelBufferLength, m_spectrumBufferLength) > m_dataLength)
                {
                    m_bufferPosition = 0;
                    m_dataLength = 0;
                    // Data needs to be read into m_buffer in order to be analysed
                    const qint64 readPos = qMax(qint64(0), qMin(levelPosition, spectrumPosition));
                    const qint64 readEnd = qMin(m_analysisFile->size(), qMax(levelPosition + m_levelBufferLength, spectrumPosition + m_spectrumBufferLength));
                    const qint64 readLen = readEnd - readPos + audioLength(m_format, WaveformWindowDuration);

                 /*   qDebug() << "Engine::audioNotify [1]"
                             << "analysisFileSize" << m_analysisFile->size()
                             << "readPos" << readPos
                             << "readLen" << readLen;*/

                    if (m_analysisFile->seek(readPos + m_analysisFile->headerLength()))
                    {
                        m_buffer.resize(readLen);
                        m_bufferPosition = readPos;
                        m_dataLength = m_analysisFile->read(m_buffer.data(), readLen);
                        //qDebug() << "Engine::audioNotify [2]" << "bufferPosition" << m_bufferPosition << "dataLength" << m_dataLength;
                    }
                    else
                    {
                       // qDebug() << "Engine::audioNotify [2]" << "file seek error";
                    }

                    if(m_dataLength != m_prevDataLength)
                    {
                        m_prevDataLength = m_dataLength;
                        Q_EMIT dataLengthChanged(m_dataLength);
                    }
                    Q_EMIT bufferChanged(m_bufferPosition, m_dataLength, m_buffer);
                }
            }
            else
            {
                if (playPosition >= m_dataLength)
                    stopPlayback();
            }
        }
        break;
    }
}

void Engine::audioStateChanged(QAudio::State state)
{
    ENGINE_DEBUG << "Engine::audioStateChanged from" << m_state
                 << "to" << state;

    if (QAudio::IdleState == state && m_file && m_file->pos() == m_file->size()) {
        stopPlayback();
    } else {
        if (QAudio::StoppedState == state) {
            // Check error
            QAudio::Error error = QAudio::NoError;
            switch (m_mode) {
            case QAudio::AudioInput:
                error = m_audioInput->error();
                break;
            case QAudio::AudioOutput:
                error = m_audioOutput->error();
                break;
            }
            if (QAudio::NoError != error) {
                reset();
                return;
            }
        }
        setState(state);
    }
}




//-----------------------------------------------------------------------------
// Private functions
//-----------------------------------------------------------------------------

void Engine::resetAudioDevices()
{
    delete m_audioInput;
    m_audioInput = 0;
    m_audioInputIODevice = 0;
    delete m_audioOutput;
    m_audioOutput = 0;
    setPlayPosition(0);
    m_spectrumPosition = 0;
}

void Engine::reset()
{
    stopPlayback();
    setState(QAudio::AudioInput, QAudio::StoppedState);
    setFormat(QAudioFormat());
    delete m_file;
    m_file = 0;
    delete m_analysisFile;
    m_analysisFile = nullptr;
    m_buffer.clear();
    m_bufferPosition = 0;
    m_bufferLength = 0;
    m_dataLength = 0;
    Q_EMIT dataLengthChanged(0);
    resetAudioDevices();
}

bool Engine::initialize()
{
    bool result = false;

    QAudioFormat format = m_format;

    if (selectFormat())
    {
        if (m_format != format)
        {
            resetAudioDevices();
            if (m_file)
            {
                Q_EMIT bufferLengthChanged(bufferLength());
                Q_EMIT dataLengthChanged(dataLength());
                Q_EMIT channelCountChanged(m_format.channelCount());
                Q_EMIT bufferChanged(0, 0, m_buffer);
                result = true;
            }
            else
            {
                m_bufferLength = audioLength(m_format, BufferDurationUs);
                m_buffer.resize(m_bufferLength);
                m_buffer.fill(0);
                Q_EMIT bufferLengthChanged(bufferLength());

                Q_EMIT bufferChanged(0, 0, m_buffer);
                m_audioInput = new QAudioInput(m_audioInputDevice, m_format, this);
                m_audioInput->setNotifyInterval(NotifyIntervalMs);
                result = true;

            }
            m_audioOutput = new QAudioOutput(m_audioOutputDevice, m_format, this);
            m_audioOutput->setNotifyInterval(NotifyIntervalMs);
            m_audioOutput->setCategory(m_audioOutputCategory);
        }
    } else {
        if (m_file)
            Q_EMIT errorMessage(tr("Audio format not supported"),
                              formatToString(m_format));
        else
            Q_EMIT errorMessage(tr("No common input / output format found"), "");
    }

    m_prevDataLength = m_dataLength;
    ENGINE_DEBUG << "Engine::initialize" << "m_bufferLength" << m_bufferLength;
    ENGINE_DEBUG << "Engine::initialize" << "m_dataLength" << m_dataLength;
    ENGINE_DEBUG << "Engine::initialize" << "format" << m_format;
    ENGINE_DEBUG << "Engine::initialize" << "m_audioOutputCategory" << m_audioOutputCategory;

    return result;
}

bool Engine::selectFormat()
{
    bool foundSupportedFormat = false;

    if (m_file || QAudioFormat() != m_format) {
        QAudioFormat format = m_format;
        if (m_file)
            // Header is read from the WAV file; just need to check whether
            // it is supported by the audio output device
            format = m_file->fileFormat();
        if (m_audioOutputDevice.isFormatSupported(format)) {
            setFormat(format);
            foundSupportedFormat = true;
        }
    } else {

        QList<int> sampleRatesList;
    #ifdef Q_OS_WIN
        // The Windows audio backend does not correctly report format support
        // (see QTBUG-9100).  Furthermore, although the audio subsystem captures
        // at 11025Hz, the resulting audio is corrupted.
        sampleRatesList += 8000;
    #endif

        sampleRatesList += m_audioOutputDevice.supportedSampleRates();
        sampleRatesList = sampleRatesList.toSet().toList(); // remove duplicates
        qSort(sampleRatesList);
        ENGINE_DEBUG << "Engine::initialize frequenciesList" << sampleRatesList;

        QList<int> channelsList;
        channelsList += m_audioInputDevice.supportedChannelCounts();
        channelsList += m_audioOutputDevice.supportedChannelCounts();
        channelsList = channelsList.toSet().toList();
        qSort(channelsList);
        ENGINE_DEBUG << "Engine::initialize channelsList" << channelsList;

        QAudioFormat format;
        format.setByteOrder(QAudioFormat::LittleEndian);
        format.setCodec("audio/pcm");
        format.setSampleSize(16);
        format.setSampleType(QAudioFormat::SignedInt);
        int sampleRate, channels;

        foreach (sampleRate, sampleRatesList)
        {
            if (foundSupportedFormat)
                break;
            format.setSampleRate(sampleRate);
            foreach (channels, channelsList)
            {
                format.setChannelCount(channels);
                const bool inputSupport = m_audioInputDevice.isFormatSupported(format);
                const bool outputSupport = m_audioOutputDevice.isFormatSupported(format);
                ENGINE_DEBUG << "Engine::initialize checking " << format
                             << "input" << inputSupport
                             << "output" << outputSupport;

                if (inputSupport && outputSupport)
                {
                    foundSupportedFormat = true;
                    break;
                }
            }
        }

        if (!foundSupportedFormat)
            format = QAudioFormat();

        setFormat(format);
    }

    return foundSupportedFormat;
}


void Engine::stopPlayback()
{
    if (m_audioOutput) {
        m_audioOutput->stop();
        QCoreApplication::instance()->processEvents();
        m_audioOutput->disconnect();
        setPlayPosition(0);
    }
}

void Engine::setState(QAudio::State state)
{
    const bool changed = (m_state != state);
    m_state = state;
    if (changed)
        Q_EMIT stateChanged(m_mode, m_state);
}

void Engine::setState(QAudio::Mode mode, QAudio::State state)
{
    const bool changed = (m_mode != mode || m_state != state);
    m_mode = mode;
    m_state = state;
    if (changed)
        Q_EMIT stateChanged(m_mode, m_state);
}


void Engine::setPlayPosition(qint64 position, bool forceEmit)
{
    const bool changed = (m_playPosition != position);
    m_playPosition = position;
    if (changed || forceEmit)
        Q_EMIT playPositionChanged(m_playPosition);
}


void Engine::setFormat(const QAudioFormat &format)
{
    const bool changed = (format != m_format);
    m_format = format;
    m_levelBufferLength = audioLength(m_format, LevelWindowUs);
    m_spectrumBufferLength = SpectrumLengthSamples *
                            (m_format.sampleSize() / 8) * m_format.channelCount();
    if (changed)
        Q_EMIT formatChanged(m_format);
}
