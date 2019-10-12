#include <QQmlEngine>

#include "include/gui/mainwidget.h"
#include "include/audio/track.h"

Track *Track::m_trackInstance = nullptr;

MainWidget::MainWidget(QString path) : jsoncare(path + "/whale_data.json", path)
{
    jsoncare.setSpecies();
    dataAbsPath = path;

    m_controls = new PlayerControls(this);
    connect(&jsoncare, &JsonCaretaker::newTrack, m_controls, &PlayerControls::loadFiles);

    m_audioWidget = new AudioWidget;
    connect(m_controls, &PlayerControls::bufferToGo, m_audioWidget, &AudioWidget::getData);
    connect(m_controls, &PlayerControls::dataLengthChanged, m_audioWidget, &AudioWidget::dataLengthChanged);
    connect(m_controls, &PlayerControls::channelCountChanged, m_audioWidget, &AudioWidget::channelCountChanged);
    connect(m_controls, &PlayerControls::newTrack, m_audioWidget, &AudioWidget::setLoadedFileLabel);

    connect(&generator, &WaveGenerator::sendData, m_audioWidget, &AudioWidget::getData);
    connect(&generator, &WaveGenerator::sendData, m_controls, &PlayerControls::generatorBuffer);

    m_waterfall = new WaterfallWidget;
    spectroThread = new QThread;
    m_spectrum.moveToThread(spectroThread);
    spectroThread->start();

    connect(m_controls, &PlayerControls::newTrack, m_waterfall, &WaterfallWidget::setFech);
    connect(m_controls, &PlayerControls::newTrack, &m_spectrum, &Spectrogram::setFech);

    connect(m_controls, &PlayerControls::bufferToGo, &m_spectrum, &Spectrogram::computeFFT);
    connect(&m_spectrum, &Spectrogram::dataReady, m_waterfall, &WaterfallWidget::plotData);

    QQuickWidget *buttonsWidget = new QQuickWidget;
    buttonsWidget->setClearColor(QColor("#dfe4ea"));
    buttonsWidget->rootContext()->setContextProperty("jsoncare", &jsoncare);
    buttonsWidget->rootContext()->setContextProperty("playercontrols", m_controls);
    buttonsWidget->rootContext()->setContextProperty("waterfall", m_waterfall);
    buttonsWidget->rootContext()->setContextProperty("wavegenerator", &generator);
    buttonsWidget->rootContext()->setContextProperty("track", Track::get());
    buttonsWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
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

MainWidget::~MainWidget()
{
    Track::destroy();
}

