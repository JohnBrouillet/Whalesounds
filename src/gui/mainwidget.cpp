#include <QQmlEngine>

#include "include/gui/mainwidget.h"
#include "include/audio/track.h"

MainWidget::MainWidget(QString path) : m_jsoncare(path + "/whale_data.json", path)
{
    m_jsoncare.setSpecies();

    m_controls = new PlayerControls(this);
    connect(&m_jsoncare, &JsonCaretaker::newTrack, m_controls, &PlayerControls::loadFiles);

    m_audioWidget = new AudioWidget;

    connect(m_controls, &PlayerControls::newTrack, m_audioWidget, &AudioWidget::setLoadedFileLabel);
    connect(m_controls, &PlayerControls::newTrack, m_audioWidget, &AudioWidget::updateGraph);
    connect(m_controls, &PlayerControls::newPosition, m_audioWidget, &AudioWidget::updateCursor);

    m_spectroThread = new QThread;
    m_spectrum.moveToThread(m_spectroThread);
    m_spectroThread->start();

    m_spectro = new SpectrogramWidget;

    connect(m_controls, &PlayerControls::newTrack, m_spectro, &SpectrogramWidget::setAxis);
    connect(m_controls, &PlayerControls::newPosition, m_spectro, &SpectrogramWidget::updateCursor);
    connect(m_controls, &PlayerControls::newTrack, &m_spectrum, &Spectrogram::computeFFT);

    connect(&m_spectrum, &Spectrogram::dataReady, m_spectro, &SpectrogramWidget::plot);

    connect(m_audioWidget, SIGNAL(axisChange(QCPRange)), m_spectro, SLOT(moveAxis(QCPRange)));
    connect(m_spectro, SIGNAL(axisChange(QCPRange)), m_audioWidget, SLOT(moveAxis(QCPRange)));


    QQuickWidget* buttonsWidget = new QQuickWidget;
    buttonsWidget->setClearColor(QColor("#dfe4ea"));
    buttonsWidget->rootContext()->setContextProperty("jsoncare", &m_jsoncare);
    buttonsWidget->rootContext()->setContextProperty("playercontrols", m_controls);
    buttonsWidget->rootContext()->setContextProperty("spectrogram", m_spectro);
    buttonsWidget->rootContext()->setContextProperty("audiograph", m_audioWidget);
    buttonsWidget->rootContext()->setContextProperty("track", Track::get());
    buttonsWidget->rootContext()->setContextProperty("layout", this);
    buttonsWidget->setResizeMode(QQuickWidget::SizeRootObjectToView);
    buttonsWidget->setSource(QUrl("qrc:qml/soundsMenu.qml"));

    m_grid = new QGridLayout;
    m_grid->addWidget(m_audioWidget, 0, 1);
    m_grid->addWidget(buttonsWidget, 0, 0, 1, 1);
    m_grid->addWidget(m_spectro, 1, 0, 1, 2);

    setLayout(m_grid);
}

void MainWidget::reverseGraph()
{
    int indexA = m_grid->indexOf(m_audioWidget);
    int indexB = m_grid->indexOf(m_spectro);
    int row1, column1, rowSpan1, columnSpan1;
    int row2, column2, rowSpan2, columnSpan2;

    m_grid->getItemPosition(indexA, &row1, &column1, &rowSpan1, &columnSpan1);
    m_grid->getItemPosition(indexB, &row2, &column2, &rowSpan2, &columnSpan2);

    QLayoutItem * tmp1 = m_grid->takeAt(indexA);
    QLayoutItem * tmp2 = m_grid->takeAt(indexB);
    delete tmp1;
    delete tmp2;

    m_grid->addWidget(m_spectro, row1, column1, rowSpan1, columnSpan1);
    m_grid->addWidget(m_audioWidget, row2, column2, rowSpan2, columnSpan2);
}

MainWidget::~MainWidget()
{
    Track::destroy();
}

