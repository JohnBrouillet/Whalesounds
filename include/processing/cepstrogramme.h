#ifndef CEPSTROGRAMME_H
#define CEPSTROGRAMME_H

#include <QWidget>

#include "3rdparties/qcustomplot/qcustomplot.h"


class CepstrogramWidget : public QWidget
{
    Q_OBJECT
public:
    CepstrogramWidget();

    void plotDesign();

/*public Q_SLOTS:
    void plotData(QVector<qreal> data);
    void setFech(int _fech);
    void zoom(bool X, bool Y);

    void zoomActivation();
    void zoomSpectro(bool value);
    void changeColorRange(bool value);

    void reinit();*/

private:
    QCustomPlot * plot;
    QCPColorMap * colorMap;
    QCPColorScale *colorScale;

    QVector<double> xAxis;

    bool zoomX, zoomY;

};

#endif // CEPSTROGRAMME_H
