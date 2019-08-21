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

#ifndef PLAYERCONTROLS_H
#define PLAYERCONTROLS_H

#include <QMediaPlayer>
#include <QWidget>
#include <QAudio>
#include <QThread>
#include <QSlider>
#include <QLabel>
#include <QTime>
#include <QAudioOutput>

#include "engine.h"
#include "bufferdecoderthread.h"
#include "tonegenerator.h"

QT_BEGIN_NAMESPACE
class QAbstractButton;
class QAbstractSlider;
class QComboBox;
QT_END_NAMESPACE

class PlayerControls : public QWidget
{
    Q_OBJECT

public:
    explicit PlayerControls(QWidget *parent = nullptr);

    QAudio::State state() const;
    qreal playbackRate() const;

private:
    void engineConnect();

public Q_SLOTS:
    void setState(QAudio::Mode mode, QAudio::State state);
    void loadFiles(QStringList filenames);
    void updateDurationInfo(qint64 currentInfo);
    void next();
    void previous();
    void playClicked();
    void reloadBuffer(bool reload)
    {
        isLoaded = reload;
        m_engine->stopPlayback();
    }


Q_SIGNALS:
    void play();
    void pause();
    void stop();
    void bufferToGo(int, QVector<double>);
    void dataLengthChanged(qint64);
    void channelCountChanged(int);
    void newFech(int);
    void newFiles();
    void newTime(QString time);
    void newText(QString _text);
    void newIndexFile(int indexFile, int nbfiles);
    void newState(bool state);
    void generatorBuffer(int i, QVector<double> data);


private:
    Engine * m_engine;
    BufferDecoderThread decoder;
    QAudio::State m_playerState = QAudio::State::StoppedState;


    QStringList filesPath;
    qint64 m_duration;
    int indexFile;

    bool isLoaded;
};

#endif // PLAYERCONTROLS_H
