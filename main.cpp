#include <QApplication>
#include <QQmlApplicationEngine>

#include <QQmlContext>
#include "settingshandler.h" // Include the header for SettingsHandler
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    // Create the QQmlApplicationEngine instance
    QQmlApplicationEngine engine;

    // Create the SettingsHandler instance
    SettingsHandler settingsHandler;

    // Expose SettingsHandler to QML using the engine's root context
    engine.rootContext()->setContextProperty("settingsHandler", &settingsHandler);

    // Load the QML file
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreationFailed,
        &app,
        []() { QCoreApplication::exit(-1); },
        Qt::QueuedConnection);
    engine.loadFromModule("untitled5", "Main");

    return app.exec();
}
