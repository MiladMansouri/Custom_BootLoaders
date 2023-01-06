#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <terminal.h>

int main(int argc, char *argv[]) {
  QGuiApplication app(argc, argv);

  QQmlApplicationEngine engine;

  Terminal terminal;
  qmlRegisterType<Terminal>("com.milad.terminal", 1, 0, "Terminal");
  const QUrl url(u"qrc:/BootLoader/main.qml"_qs);
  QObject::connect(
      &engine, &QQmlApplicationEngine::objectCreated, &app,
      [url](QObject *obj, const QUrl &objUrl) {
        if (!obj && url == objUrl)
          QCoreApplication::exit(-1);
      },
      Qt::QueuedConnection);
  engine.load(url);

  return app.exec();
}
