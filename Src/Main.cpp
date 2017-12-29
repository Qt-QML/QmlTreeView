#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include "TreeModel.hpp"
int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    QGuiApplication app(argc, argv);

    QQuickView view;
    TreeModel model;
    model.loadDataFromJson(":/Data.json");
    view.rootContext()->setContextProperty("tModel", &model);
    view.setSource(QUrl(QLatin1String("qrc:/Qml/Main.qml")));
    view.show();

    return app.exec();
}
