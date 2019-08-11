#include "include/gui/mainwidget.h"

MainWidget::MainWidget(QString path) : jsoncare(path + "/whale_data.json", path)
{
    jsoncare.setSpecies();
    dataAbsPath = path;
    connect(&jsoncare, SIGNAL(soundsPath(QStringList)), this, SLOT(setFiles(QStringList)));

    m_controls = new PlayerControls(this);


    m_audioWidget = new AudioWidget;
    connect(m_controls, &PlayerControls::bufferToGo, m_audioWidget, &AudioWidget::getData);
    connect(m_controls, &PlayerControls::dataLengthChanged, m_audioWidget, &AudioWidget::dataLengthChanged);
    connect(m_controls, &PlayerControls::channelCountChanged, m_audioWidget, &AudioWidget::channelCountChanged);

    connect(&generator, &WaveGenerator::sendData, m_audioWidget, &AudioWidget::getData);
    connect(&generator, &WaveGenerator::sendData, m_controls, &PlayerControls::generatorBuffer);


    m_waterfall = new WaterfallWidget;
    spectroThread = new QThread;
    m_spectrum.moveToThread(spectroThread);
    spectroThread->start();

    connect(m_controls, &PlayerControls::newFech, m_waterfall, &WaterfallWidget::setFech);
    connect(m_controls, &PlayerControls::newFech, &m_spectrum, &Spectrogram::setFech);

    connect(m_controls, &PlayerControls::bufferToGo, &m_spectrum, &Spectrogram::computeFFT);
    connect(&m_spectrum, &Spectrogram::dataReady, m_waterfall, &WaterfallWidget::plotData);


    QQuickWidget *buttonsWidget = new QQuickWidget;
    buttonsWidget->setClearColor(QColor("#dfe4ea"));
    buttonsWidget->rootContext()->setContextProperty("jsoncare", &jsoncare);
    buttonsWidget->rootContext()->setContextProperty("playercontrols", m_controls);
    buttonsWidget->rootContext()->setContextProperty("waterfall", m_waterfall);
    buttonsWidget->rootContext()->setContextProperty("wavegenerator", &generator);
    buttonsWidget->setSource(QUrl("qrc:qml/soundsMenu.qml"));

#ifndef AUDIO_ONLY
    QQuickWidget *optionsWidget = new QQuickWidget;
    QRect screenres = QApplication::desktop()->screenGeometry(2);
    optionsWidget->setSource(QUrl("qrc:qml/OptionsWindow.qml"));
    optionsWidget->setClearColor(QColor("#dfe4ea"));

    optionsWidget->move(QPoint(screenres.x()+1000, screenres.y()+500));
    optionsWidget->show();
#endif

    QGridLayout * grid = new QGridLayout;
    grid->addWidget(buttonsWidget, 0, 0, 2, 1);

    grid->addWidget(m_audioWidget, 0, 1);
    grid->addWidget(m_waterfall, 1, 1);

    setLayout(grid);
}


void MainWidget::setImage(QString path)
{
    QPixmap pix(path);


}

void MainWidget::setFiles(QStringList path)
{
    m_controls->loadFiles(path);
}

void MainWidget::openFileNameReady()
{
    qDebug() << "get buf";
}

void MainWidget::seek(int seconds)
{
   // m_player->setPosition(seconds * 1000);
}

void MainWidget::durationChanged(qint64 duration)
{
    m_duration = duration / 1000;
   // m_slider->setMaximum(m_duration);
}

void MainWidget::positionChanged(qint64 progress)
{
    //if (!m_slider->isSliderDown())
    //    m_slider->setValue(progress / 1000);

    updateDurationInfo(progress / 1000);
}

void MainWidget::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || m_duration) {
        QTime currentTime((currentInfo / 3600) % 60, (currentInfo / 60) % 60,
            currentInfo % 60, (currentInfo * 1000) % 1000);
        QTime totalTime((m_duration / 3600) % 60, (m_duration / 60) % 60,
            m_duration % 60, (m_duration * 1000) % 1000);
        QString format = "mm:ss";
        if (m_duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }


}
