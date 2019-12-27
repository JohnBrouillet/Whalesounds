#include "include/gui/mainwidget.h"

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

    QQuickStyle::setStyle("Imagine");
    a.setStyleSheet(styleSheet);

    MainWidget w(path);

    QRect screenres = QApplication::desktop()->screenGeometry(0/*screenNumber*/);
    w.move(QPoint(screenres.x(), screenres.y()));
    w.resize(screenres.width(), screenres.height());
    w.show();

    return a.exec();
}


