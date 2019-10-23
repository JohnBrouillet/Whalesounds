#ifndef ABSTRACTPLOT_H
#define ABSTRACTPLOT_H

#include "3rdparties/qcustomplot/qcustomplot.h"

class AbstractPlot : public QWidget
{
    Q_OBJECT

protected:
    AbstractPlot();
    void design();

public Q_SLOTS:
    void updateCursor(double position);
    void zoom(bool X, bool Y);
    void zoomActivation();
    void zoomSpectro(bool value);

protected:
    QCustomPlot * m_plot;
    QCPItemLine * m_cursor;

    bool m_zoomX, m_zoomY;
};

#endif // ABSTRACTPLOT_H
