#include <QApplication>
#include <QCommandLineParser>
#include <QCommandLineOption>
#include <QSplashScreen>
#include <QImage>

#include "mainwindow.h"

int main(int argc, char *argv[])
{


    QApplication app(argc, argv);
    MainWindow mainWin;
    //Q_INIT_RESOURCE(application);

//    //initial COOLANG_HOME env-variable
//    QString appDirPath = QApplication::applicationDirPath();
//    //QString COOLANG_HOME = appDirPath+"/COOLANG_HOME";
//    //qputenv("PATH", COOLANG_HOME.toStdString().c_str());
    QCoreApplication::setOrganizationName("COOL Programming Community");
    QCoreApplication::setApplicationName("Chain-Oriented Objective Logic");
    QCoreApplication::setApplicationVersion(QT_VERSION_STR);
    QFont f("Arial",12);                        //设置显示的字体格式
    app.setFont(f);
    QImage img(":/images/icon/opening_icon.png");//设置开场动画图片，可以是静态图片，也可是gif
    QPixmap pix;
//    pix = pix.fromImage(img.scaled(400,225,Qt::IgnoreAspectRatio,Qt::FastTransformation));//设置图片大小
//    pix.scaled(500,200);
    pix = pix.fromImage(img);//设置图片大小
   // pix.scaled(500,200);
    QSplashScreen splash(pix);
    splash.show();

    QEventLoop eventloop1;
    QTimer::singleShot(1000,&eventloop1,SLOT(quit()));//设置个延时，看得清楚点
    eventloop1.exec();

    app.processEvents();

//    if (!parser.positionalArguments().isEmpty())
//        w.loadFile(parser.positionalArguments().first());
    splash.finish(&mainWin);
    app.setWindowIcon(QIcon(":/icon.ico"));
    mainWin.setWindowTitle("Chain-Oriented Objective Logic");
    QObject::connect(&app, &QApplication::aboutToQuit, [&mainWin]() {
        mainWin.terminatesubProcesses();
    });
    mainWin.show();
    return app.exec();
}
