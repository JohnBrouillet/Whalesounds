#include "include/gui/spectrogramWidget.h"
#include "include/audio/spectrum.h"
#include "include/audio/track.h"

#include <iostream>

SpectrogramWidget::SpectrogramWidget()
{
    m_colorMap = new QCPColorMap(m_plot->xAxis, m_plot->yAxis);
    m_colorScale = new QCPColorScale(m_plot);

    // delete and recreate cursor to put it on top of the color map
    delete m_cursor;
    m_cursor = new QCPItemLine(m_plot);
    m_cursor->setPen(QPen(Qt::white));
    m_cursor->start->setCoords(0, -QCPRange::maxRange);
    m_cursor->end->setCoords(0, QCPRange::maxRange);

    design();
}

void SpectrogramWidget::plot(QVector<qreal> data)
{

    for(int xIndex = 0; xIndex < m_nbLines; xIndex++)
    {
        for(int yIndex = 0; yIndex < NFFT/2; yIndex++)
            m_colorMap->data()->setCell(xIndex, yIndex, data[xIndex * NFFT/2 + yIndex]);
    }

    m_colorMap->rescaleDataRange(true);
    m_plot->replot();
}

void SpectrogramWidget::setAxis()
{
    m_colorMap->data()->clear();
    //m_plot->graph(0)->data()->clear();
    int fech = Track::get()->getFormat().sampleRate();

    m_nbLines = Track::get()->getData().size() / double( temporalResolution * fech ) - 1;

    m_colorMap->data()->setSize(m_nbLines, NFFT/2);
    m_colorMap->data()->setRange(QCPRange(0, m_nbLines*temporalResolution), QCPRange(0, fech/2));

    m_plot->rescaleAxes();
    m_plot->replot();
}

void SpectrogramWidget::reinit()
{
    m_colorMap->rescaleDataRange();
    m_plot->replot();
}

void SpectrogramWidget::design()
{
    m_plot->yAxis->setLabel("Frequencies (Hz)");
    m_plot->yAxis->setLabelColor(QColor("6b7b89"));
    m_plot->yAxis->setTickLabels(true);

    m_colorScale->setGradient(QCPColorGradient::gpPolar);
    m_colorScale->setDataRange(QCPRange(0, 350));
    m_colorScale->setType(QCPAxis::atRight); // scale shall be vertical bar with tick/axis labels right (actually atRight is already the default)
    m_colorMap->setColorScale(m_colorScale); // associate the color map with the color scale

    m_plot->plotLayout()->addElement(0, 1, m_colorScale);

    QCPMarginGroup *marginGroup = new QCPMarginGroup(m_plot);
    m_plot->axisRect()->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    m_colorScale->setMarginGroup(QCP::msBottom|QCP::msTop, marginGroup);
    // rescale the key (x) and value (y) axes so the whole color map is visible:
    m_plot->rescaleAxes();
}

void SpectrogramWidget::changeColorRange(bool value)
{
    double factor = getZoomFactor(value);

    QCPRange range = m_colorScale->dataRange();
    QCPRange newRange;
    newRange.lower = qPow(range.lower/m_plot->yAxis->range().center(), factor)*m_plot->yAxis->range().center();
    newRange.upper = qPow(range.upper/m_plot->yAxis->range().center(), factor)*m_plot->yAxis->range().center();

    m_colorScale->setDataRange(newRange);
    m_plot->replot();
}

void SpectrogramWidget::changeColor(int color)
{
    m_cursor->setPen(QPen(Qt::black));
    switch(color)
    {
    case 0: m_colorScale->setGradient(QCPColorGradient::gpGrayscale);
            break;
    case 1: m_colorScale->setGradient(QCPColorGradient::gpHot);
            break;
    case 2: m_colorScale->setGradient(QCPColorGradient::gpCold);
            break;
    case 3: m_colorScale->setGradient(QCPColorGradient::gpNight);
            break;
    case 4: m_colorScale->setGradient(QCPColorGradient::gpCandy);
            break;
    case 5: m_colorScale->setGradient(QCPColorGradient::gpGeography);
            break;
    case 6: m_colorScale->setGradient(QCPColorGradient::gpIon);
            break;
    case 7: m_colorScale->setGradient(QCPColorGradient::gpThermal);
            break;
    case 8: m_colorScale->setGradient(QCPColorGradient::gpPolar);
            m_cursor->setPen(QPen(Qt::white));
            break;
    case 9: m_colorScale->setGradient(QCPColorGradient::gpSpectrum);
            break;
    case 10: m_colorScale->setGradient(QCPColorGradient::gpJet);
            break;
    case 11: m_colorScale->setGradient(QCPColorGradient::gpHues);
            break;
    default: m_colorScale->setGradient(QCPColorGradient::gpPolar);
        break;
    }

    m_plot->replot();
}

void SpectrogramWidget::reverseColor()
{
    m_colorScale->setGradient(m_colorScale->gradient().inverted());
    m_plot->replot();
}
