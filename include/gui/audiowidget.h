#ifndef AUDIOWIDGET_H
#define AUDIOWIDGET_H

#include "include/gui/abstractPlot.h"
#include "3rdparties/qcustomplot/qcustomplot.h"

class AudioWidget : public AbstractPlot
{
    Q_OBJECT

public:
    AudioWidget();

public Q_SLOTS:
    void updateGraph();
    void setLoadedFileLabel();

private:
    void design();

private:
    QCPTextElement* m_title;

};

#endif // AUDIOWIDGET_H
