#include "include/gui/mainwidget.h"
#include "include/utils/jsoncaretaker.h"
#include "include/audio/track.h"
#include "include/utils/widgetShower.h"

#include <QApplication>
#include <QtQuickControls2>
#include <QIcon>


#ifdef Q_OS_ANDROID
#include <QAndroidJniObject>
#include <QAndroidJniEnvironment>
#include <QtAndroid>
#endif

#ifdef Q_OS_IOS
#include <QStandardPaths>
#endif

int main(int argc, char *argv[])
{
    QString path;

#ifdef ANDROID
    QAndroidJniObject mediaDir = QAndroidJniObject::callStaticObjectMethod("android/os/Environment", "getExternalStorageDirectory", "()Ljava/io/File;");
    QAndroidJniObject mediaPath = mediaDir.callObjectMethod( "getAbsolutePath", "()Ljava/lang/String;" );
    path = mediaPath.toString() + "/whalesounds/";

    QAndroidJniEnvironment env;
    if (env->ExceptionCheck()) {
            // Handle exception here.
            env->ExceptionClear();
    }

    auto  result = QtAndroid::checkPermission(QString("android.permission.READ_EXTERNAL_STORAGE"));
    if(result == QtAndroid::PermissionResult::Denied)
    {
       QtAndroid::PermissionResultMap resultHash = QtAndroid::requestPermissionsSync(QStringList({"android.permission.READ_EXTERNAL_STORAGE"}));
       if(resultHash["android.permission.READ_EXTERNAL_STORAGE"] == QtAndroid::PermissionResult::Denied)
           return 0;
    }
#elif TARGET_OS_IOS
    QStringList pathList = QStandardPaths::standardLocations(QStandardPaths::DocumentsLocation);
    path = pathList[0] + "/data";
#else
    path = QDir::currentPath() + "/data";
#endif

    qDebug() << path;

    QApplication a(argc, argv);
    QFile file(":/qml/design.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());

    QIcon::setThemeName("whalesounds");

    QQuickStyle::setStyle("Material");
    a.setStyleSheet(styleSheet);

    JsonCaretaker jsoncare(path + "/whale_data.json", path);
    jsoncare.setSpecies();

    WidgetShower shower(path, jsoncare);

    QQuickWidget *tileView = new QQuickWidget;
    tileView->setClearColor(QColor("#dfe4ea"));
    tileView->rootContext()->setContextProperty("jsoncare", &jsoncare);
    tileView->rootContext()->setContextProperty("shower", &shower);
    tileView->setResizeMode(QQuickWidget::SizeRootObjectToView);
    tileView->setSource(QUrl("qrc:qml/TileView.qml"));
    tileView->show();


    /*MainWidget w(path, jsoncare);

    QRect screenres = QApplication::desktop()->screenGeometry(1);
    w.move(QPoint(screenres.x(), screenres.y()));
    w.resize(screenres.width(), screenres.height());
    w.show();*/

    return a.exec();
}


