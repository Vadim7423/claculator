#include <QGuiApplication>
#include <QQmlApplicationEngine>

#include "src/calculator.h"

QObject *calc_instance(QQmlEngine *engine, QJSEngine *scriptEngine)
{
    Calculator &state_container = Calculator::instance();
    Q_UNUSED(engine);
    Q_UNUSED(scriptEngine);
    engine->setObjectOwnership(&state_container, QQmlEngine::CppOwnership);
    return &state_container;
}

int main(int argc, char *argv[])
{
    QGuiApplication app(argc, argv);
    QGuiApplication::setOrganizationName("calc");
    QGuiApplication::setOrganizationDomain("calc.ru");
    QGuiApplication::setApplicationName("Calculator");
    qmlRegisterSingletonType<Calculator>("App", 1, 0, "Calculator", &calc_instance);
    QQmlApplicationEngine engine;
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    //view.setSource(QUrl(QStringLiteral("qrc:/Main.qml")));
    //engine.loadFromModule("calculator", "Main");
    engine.load(QUrl(QStringLiteral("qrc:/Main.qml")));
    return app.exec();
}
