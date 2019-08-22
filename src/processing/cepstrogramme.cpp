#include "include/processing/cepstrogramme.h"

static const int NFFT = 8*2048;
static int fech = 44100;
static const int maxFreqDisp = 15000;

CepstrogramWidget::CepstrogramWidget()
{
    plot = new QCustomPlot;
    zoomX = false;
    zoomY = false;
    //zoom(0,0);

    plotDesign();
    plot->addGraph();

    QGridLayout * layout = new QGridLayout;
    layout->addWidget(plot, 0, 0);

    setLayout(layout);
}

void CepstrogramWidget::plotDesign()
{

    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(223, 228, 234, 92));
    plotGradient.setColorAt(1, QColor(223, 228, 234, 92));
    plot->setBackground(QBrush(QColor("#dfe4ea")));

    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(255, 255, 255));
    axisRectGradient.setColorAt(1, QColor("#dfe4ea"));
    plot->axisRect()->setBackground(QColor("#ffffff"));

    plot->xAxis->setBasePen(QPen(QColor("6b7b89"), 1));
    //plot->yAxis->setBasePen(QPen(QColor("6b7b89"), 1));
    plot->xAxis->setTickPen(QPen(QColor("6b7b89"), 1));
    //plot->yAxis->setTickPen(QPen(QColor("6b7b89"), 1));
    plot->xAxis->setSubTickPen(QPen(QColor("6b7b89"), 1));
    //plot->yAxis->setSubTickPen(QPen(QColor("6b7b89"), 1));
    plot->xAxis->setTickLabelColor(QColor("6b7b89"));
    //plot->yAxis->setTickLabelColor(QColor("6b7b89"));

    QSharedPointer<QCPAxisTicker> xticker(new QCPAxisTicker);
    xticker->setTickCount(5);
    xticker->setTickStepStrategy(QCPAxisTicker::tssReadability);
    plot->xAxis->setTicker(xticker);

    plot->xAxis->setLabel("Frequencies (Hz)");
    plot->xAxis->setLabelColor(QColor("6b7b89"));

    colorMap = new QCPColorMap(plot->xAxis, plot->yAxis);
    int ny = 200;
    int nx = int(maxFreqDisp * NFFT / fech);
    colorMap->data()->setSize(nx, ny);
    colorMap->data()->setRange(QCPRange(0, maxFreqDisp), QCPRange(0, 200));

    colorScale = new QCPColorScale(plot);
    colorScale->setGradient(QCPColorGradient::gpThermal);

    colorScale->setDataRange(QCPRange(0, 5));
    colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
    colorMap->setColorScale(colorScale); // associate the color map with the color scale

    plot->plotLayout()->addElement(0, 1, colorScale);      // add it to the right of the main axis rect

    //colorMap->rescaleDataRange();

    QCPMarginGroup *marginGroup = new QCPMarginGroup(plot);
    plot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);

    // rescale the key (x) and value (y) axes so the whole color map is visible:
    plot->rescaleAxes();
}
