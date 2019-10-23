#ifndef SPECTROGRAMWIDGET_H
#define SPECTROGRAMWIDGET_H

#include "include/gui/abstractPlot.h"

class SpectrogramWidget : public AbstractPlot
{
    Q_OBJECT

public:
    SpectrogramWidget();
    void design();

public Q_SLOTS:
    void plot(QVector<qreal> data);
    void setAxis();
    void reinit();
    void changeColorRange(bool value);
    void changeColor(int color);
    void reverseColor();

private:
    QCPColorMap * m_colorMap;
    QCPColorScale * m_colorScale;

    int m_nbLines;

};


#endif // SPECTROGRAMWIDGET_H
