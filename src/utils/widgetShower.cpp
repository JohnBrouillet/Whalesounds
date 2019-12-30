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
