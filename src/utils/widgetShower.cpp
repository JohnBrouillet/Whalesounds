#include "include/utils/widgetShower.h"

WidgetShower::WidgetShower(QString path, JsonCaretaker& jsoncare) : m_jsoncare(jsoncare), m_mainWidget(path, jsoncare)
{
    m_mainWidget.setShower(this);
    m_path = path;
}


void WidgetShower::show()
{
   /* QRect screenres = QApplication::desktop()->screenGeometry(1);
    m_mainWidget.move(QPoint(screenres.x(), screenres.y()));
    m_mainWidget.resize(screenres.width(), screenres.height());*/
    m_mainWidget.show();
}

void WidgetShower::unshow()
{
    m_mainWidget.hide();
}

QString WidgetShower::durationText(qint64 position, qint64 _duration)
{
    QString timeString;
    qint64 time = position / 1000;
    qint64 duration = _duration / 1000;
    if (position || duration)
    {
        QTime currentTime((time / 3600) % 60, (time / 60) % 60, time % 60, (time * 1000) % 1000);
        QTime totalTime((duration / 3600) % 60, (duration / 60) % 60, duration % 60, (duration * 1000) % 1000);
        QString format = "mm:ss";
        if (duration > 3600)
            format = "hh:mm:ss";
        else if (duration < 60)
            format = "ss";
        timeString = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    else
        timeString = "00/00";

    return timeString;
}
