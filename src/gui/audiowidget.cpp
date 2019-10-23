#include "include/gui/audiowidget.h"
#include "include/utils/utils.h"
#include "include/audio/track.h"

AudioWidget::AudioWidget()
{
    m_title = new QCPTextElement(m_plot, "", QFont("helvetica", 11, QFont::Normal));
    m_plot->plotLayout()->insertRow(0);
    m_title->setTextColor(QColor(102,108,118));
    m_plot->plotLayout()->addElement(0, 0, m_title);

    design();
}

void AudioWidget::updateGraph()
{
    QVector<QVector<qreal>> data = Track::get()->getData();
    QAudioFormat format = Track::get()->getFormat();

    double resolution = double(1.0/format.sampleRate());

    m_plot->graph(0)->data()->clear();
    for(int i = 0; i < data[0].size(); i++)
        m_plot->graph(0)->addData(i*resolution, data[0][i]);

    m_plot->xAxis->setRange(0, data[0].size()*resolution);

    m_plot->replot();
}

void AudioWidget::setLoadedFileLabel()
{
    m_title->setText(Track::get()->getName());
}

void AudioWidget::design()
{
    m_plot->setInteractions(QCP::iRangeZoom);
    m_plot->axisRect()->setRangeZoom(Qt::Horizontal);

    m_plot->xAxis->setBasePen(QPen(QColor("6b7b89"), 1));
    m_plot->yAxis->setBasePen(QPen(QColor("6b7b89"), 1));

    QSharedPointer<QCPAxisTicker> xticker(new QCPAxisTicker);
    xticker->setTickCount(1);
    m_plot->yAxis->setTicker(xticker);

    m_plot->yAxis->setTickPen(QPen(QColor("6b7b89"), 1));
    m_plot->yAxis->setSubTickPen(QPen(QColor("6b7b89"), 1));
    m_plot->xAxis->setTickLabelColor(QColor("6b7b89"));
    m_plot->yAxis->setTickLabelColor(QColor("6b7b89"));
    m_plot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::SolidLine));
    m_plot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::SolidLine));
    m_plot->xAxis->grid()->setSubGridVisible(false);
    m_plot->yAxis->grid()->setSubGridVisible(false);
    m_plot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    m_plot->yAxis->grid()->setZeroLinePen(Qt::NoPen);

    m_plot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    m_plot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    m_plot->yAxis->setLabel("Audio level");
    m_plot->yAxis->setLabelColor(QColor("6b7b89"));
    m_plot->yAxis->setRange(-0.7 , 0.7);

    m_plot->graph(0)->setPen(QColor("#66DEFF"));
}

