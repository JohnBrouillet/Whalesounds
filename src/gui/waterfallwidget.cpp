#include "include/gui/waterfallwidget.h"
#include "include/audio/spectrum.h"
#include "include/audio/track.h"

WaterfallWidget::WaterfallWidget()
{
    plot = new QCustomPlot;
    fech = 44100;
    ny = 1000;
    zoomX = false;
    zoomY = false;
    zoom(0,0);

    plotDesign();
    plot->addGraph();

    QGridLayout * layout = new QGridLayout;
    layout->addWidget(plot, 0, 0);

    setLayout(layout);
}

void WaterfallWidget::setFech()
{
    fech = Track::get()->getFech();
    nx = int(maxFreqDisp * NFFT / fech);
    colorMap->data()->setSize(nx, ny);
    colorMap->data()->setRange(QCPRange(0, maxFreqDisp), QCPRange(0, ny));
}

void WaterfallWidget::plotData(QVector<qreal> data)
{
    colorMap->data()->insertValues(data, true);

    plot->replot();
}

void WaterfallWidget::zoomActivation()
{
    plot->xAxis->scaleRange(.85, plot->xAxis->range().center());
    plot->replot();
    if(zoomX && !zoomY)
        zoom(1,0);
    else if (zoomX && zoomY)
        zoom(1,1);
    else if (!zoomX && zoomY)
        zoom(0,1);
    else
        zoom(0,0);
}

void WaterfallWidget::zoom(bool X, bool Y)
{
    plot->setInteractions(QCP::iRangeZoom);

    zoomX = X;
    zoomY = Y;

    if(X && !Y)
    {
        plot->setInteraction(QCP::iRangeDrag, true);
        plot->axisRect()->setRangeDrag(Qt::Horizontal);
        plot->axisRect()->setRangeZoom(Qt::Horizontal);
    }
    else if(X && Y)
    {
        plot->setInteraction(QCP::iRangeDrag, true);
        plot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
        plot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    }
    else if (!X && Y)
    {
        plot->setInteraction(QCP::iRangeDrag, true);
        plot->axisRect()->setRangeDrag(Qt::Vertical);
        plot->axisRect()->setRangeZoom(Qt::Vertical);
    }
    else
        plot->setInteraction(QCP::iRangeDrag, false);
}

void WaterfallWidget::zoomSpectro(bool value)
{
    double factor;
    if(value)
        factor = 0.85;
    else
        factor = 1.17647;

    if(zoomX && !zoomY)
        plot->xAxis->scaleRange(factor, plot->xAxis->range().center());
    else if (zoomX && zoomY)
    {
        plot->xAxis->scaleRange(factor, plot->xAxis->range().center());
        plot->yAxis->scaleRange(factor, plot->yAxis->range().center());
    }
    else if (!zoomX && zoomY)
        plot->yAxis->scaleRange(factor, plot->yAxis->range().center());

    plot->replot();
}

void WaterfallWidget::plotDesign()
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
    nx = int(maxFreqDisp * NFFT / fech);
    colorMap->data()->setSize(nx, ny);
    colorMap->data()->setRange(QCPRange(0, maxFreqDisp), QCPRange(ny/2, ny));

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

void WaterfallWidget::changeColorRange(bool value)
{
    double factor;
    if(value)
        factor = 0.85;
    else
        factor = 1.17647;

    QCPRange range = colorScale->dataRange();
    QCPRange newRange;
    newRange.lower = qPow(range.lower/plot->yAxis->range().center(), factor)*plot->yAxis->range().center();
    newRange.upper = qPow(range.upper/plot->yAxis->range().center(), factor)*plot->yAxis->range().center();

    colorScale->setDataRange(newRange);


    plot->replot();
}

void WaterfallWidget::reinit()
{
    colorScale->setDataRange(QCPRange(0, 5));
    plot->replot();
}
