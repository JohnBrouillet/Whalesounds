#include "include/gui/abstractPlot.h"

AbstractPlot::AbstractPlot() : m_plot(new QCustomPlot)
{
    m_zoomX = false;
    m_zoomY = false;
    zoom(0,0);

    design();
    m_plot->addGraph();

    m_cursor = new QCPItemLine(m_plot);
    m_cursor->setPen(QPen(Qt::black));
    m_cursor->start->setCoords(0, -QCPRange::maxRange);
    m_cursor->end->setCoords(0, QCPRange::maxRange);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(m_plot);
    setLayout(mainLayout);
}

void AbstractPlot::updateCursor(double position)
{
    m_cursor->start->setCoords(position, -QCPRange::maxRange);
    m_cursor->end->setCoords(position, QCPRange::maxRange);
    m_plot->replot();
}

void AbstractPlot::zoomActivation()
{
    m_plot->xAxis->scaleRange(.85, m_plot->xAxis->range().center());
    m_plot->replot();
    if(m_zoomX && !m_zoomY)
        zoom(1,0);
    else if (m_zoomX && m_zoomY)
        zoom(1,1);
    else if (!m_zoomX && m_zoomY)
        zoom(0,1);
    else
        zoom(0,0);
}

void AbstractPlot::zoom(bool X, bool Y)
{
    m_plot->setInteractions(QCP::iRangeZoom);

    m_zoomX = X;
    m_zoomY = Y;

    if(X && !Y)
    {
        m_plot->setInteraction(QCP::iRangeDrag, true);
        m_plot->axisRect()->setRangeDrag(Qt::Horizontal);
        m_plot->axisRect()->setRangeZoom(Qt::Horizontal);
    }
    else if(X && Y)
    {
        m_plot->setInteraction(QCP::iRangeDrag, true);
        m_plot->axisRect()->setRangeDrag(Qt::Horizontal | Qt::Vertical);
        m_plot->axisRect()->setRangeZoom(Qt::Horizontal | Qt::Vertical);
    }
    else if (!X && Y)
    {
        m_plot->setInteraction(QCP::iRangeDrag, true);
        m_plot->axisRect()->setRangeDrag(Qt::Vertical);
        m_plot->axisRect()->setRangeZoom(Qt::Vertical);
    }
    else
        m_plot->setInteraction(QCP::iRangeDrag, false);
}

void AbstractPlot::zoomSpectro(bool value)
{
    double factor = getZoomFactor(value);

    if(m_zoomX && !m_zoomY)
        m_plot->xAxis->scaleRange(factor, m_plot->xAxis->range().center());
    else if (m_zoomX && m_zoomY)
    {
        m_plot->xAxis->scaleRange(factor, m_plot->xAxis->range().center());
        m_plot->yAxis->scaleRange(factor, m_plot->yAxis->range().center());
    }
    else if (!m_zoomX && m_zoomY)
        m_plot->yAxis->scaleRange(factor, m_plot->yAxis->range().center());

    m_plot->replot();
}

double AbstractPlot::getZoomFactor(bool zoom)
{
    return zoom ? 0.85 : 1.17647;
}

void AbstractPlot::design()
{
    m_plot->setBackground(QBrush(QColor("#dfe4ea")));
    m_plot->axisRect()->setBackground(QColor("#ffffff"));

    QSharedPointer<QCPAxisTickerTime> timeTicker(new QCPAxisTickerTime);
    timeTicker->setTimeFormat("%m:%s");
    timeTicker->setTickCount(5);
    m_plot->xAxis->setTicker(timeTicker);
    m_plot->setInteraction(QCP::iRangeDrag, true);
    m_plot->axisRect()->setRangeDrag(Qt::Horizontal);
    m_plot->xAxis->setLabel("Time");
}

