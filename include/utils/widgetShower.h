#ifndef WIDGETSHOWER_H
#define WIDGETSHOWER_H

#include <QObject>
#include "include/gui/mainwidget.h"
#include "include/utils/jsoncaretaker.h"

class WidgetShower : public QObject
{
    Q_OBJECT

public:
    WidgetShower(QString path, JsonCaretaker& jsoncare);

public Q_SLOTS:
    void show();
    void unshow();

private:
    QString m_path;
    JsonCaretaker& m_jsoncare;
    MainWidget m_mainWidget;
};

#endif // WIDGETSHOWER_H
