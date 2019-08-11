#ifndef WATERFALLWIDGET_H
#define WATERFALLWIDGET_H

#include "include/gui/qcustomplot.h"

class WaterfallWidget : public QWidget
{
    Q_OBJECT
public:
    WaterfallWidget();

    void plotDesign();

public Q_SLOTS:
    void plotData(QVector<qreal> data);
    void setFech(int _fech);
    void zoom(bool X, bool Y);

    void zoomActivation();
    void zoomSpectro(bool value);
    void changeColorRange(bool value);

    void reinit();

private:
    QCustomPlot * plot;
    QCPColorMap * colorMap;
    QCPColorScale *colorScale;

    QVector<double> xAxis;

    double fech;
    bool zoomX, zoomY;

};

#endif // WATERFALLWIDGET_H
