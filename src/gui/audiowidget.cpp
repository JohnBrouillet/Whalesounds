#include "include/gui/audiowidget.h"
#define MSG(text) std::cout << text << std::endl;

AudioWidget::AudioWidget()
{
    init = false;
    sampleCount = 2000;
    channelCount = 2;
    for(int i = 0; i < sampleCount; i++)
        xAxis.push_back(i);

    plot = new QCustomPlot;

    plot->plotLayout()->insertRow(0);
    text = new QCPTextElement(plot, "", QFont("helvetica", 11, QFont::Normal));
    text->setTextColor(QColor(102,108,118));
    plot->plotLayout()->addElement(0, 0, text);

    plot->addGraph();

    plotDesign();

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(plot);
    setLayout(mainLayout);
}

void AudioWidget::setLoadedFileLabel(QString specie)
{
    text->setText(specie);
}

void AudioWidget::dataLengthChanged(qint64 value)
{
    sampleCount = value / (2*channelCount);

    xAxis.clear();
    for(int i = 0; i < sampleCount; i++)
        xAxis.push_back(i);

   plot->xAxis->setRange(0, sampleCount);
}

void AudioWidget::channelCountChanged(int value)
{
    channelCount = value;
}

void AudioWidget::getBuffer(const QAudioBuffer &buffer)
{
    if(!init)
    {
        for(int i = 0; i < buffer.format().channelCount(); i++)
        {
            plot->addGraph();
        }

        init = true;
    }

}

void AudioWidget::getData(int serieIdx, QVector<double> data)
{
    if(xAxis.size() != data.size())
        dataLengthChanged(data.size() * 2 * channelCount);

    plot->graph(serieIdx)->setData(xAxis, data);
    plot->replot();
}

void AudioWidget::plotDesign()
{

    plot->setInteractions(QCP::iRangeZoom);
    plot->axisRect()->setRangeZoom(Qt::Horizontal);


    plot->xAxis->setBasePen(QPen(QColor("6b7b89"), 1));
    plot->yAxis->setBasePen(QPen(QColor("6b7b89"), 1));


    QSharedPointer<QCPAxisTicker> xticker(new QCPAxisTicker);
    xticker->setTickCount(1);
    plot->yAxis->setTicker(xticker);


   // plot->xAxis->setTickPen(QPen(QColor("6b7b89"), 1));
    plot->yAxis->setTickPen(QPen(QColor("6b7b89"), 1));
    //plot->xAxis->setSubTickPen(QPen(QColor("6b7b89"), 1));
    plot->yAxis->setSubTickPen(QPen(QColor("6b7b89"), 1));
    plot->xAxis->setTickLabelColor(QColor("6b7b89"));
    plot->yAxis->setTickLabelColor(QColor("6b7b89"));
   // plot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::SolidLine));
    plot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::SolidLine));
   // plot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::SolidLine));
    plot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::SolidLine));
    plot->xAxis->grid()->setSubGridVisible(false);
    plot->yAxis->grid()->setSubGridVisible(false);
    plot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    plot->yAxis->grid()->setZeroLinePen(Qt::NoPen);

    plot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    plot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);

    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor("#fefefe"));
    plotGradient.setColorAt(1, QColor("#f0f2f5"));
    plot->setBackground(QBrush(QColor("#dfe4ea")));

    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(255, 255, 255));
    axisRectGradient.setColorAt(1, QColor("#dfe4ea"));
    plot->axisRect()->setBackground(QColor("#ffffff"));


    plot->xAxis->setLabel("Audio samples");
    plot->xAxis->setLabelColor(QColor("6b7b89"));
    plot->xAxis->setRange(0, sampleCount);

    plot->yAxis->setLabel("Audio level");
    plot->yAxis->setLabelColor(QColor("6b7b89"));
    plot->yAxis->setRange(-0.7 , 0.7);

    plot->graph(0)->setPen(QColor("#66DEFF"));


}
