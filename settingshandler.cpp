#include "settingshandler.h"
#include <QDir>

SettingsHandler::SettingsHandler(QObject *parent) : QObject(parent) {
    QString currentPath = QDir::currentPath(); // Current working directory
    QDir dir(currentPath);
    dir.cdUp(); // Navigate up to find the project directory
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
    dir.cdUp();
    QString projectPath = dir.absolutePath() + "/config.cfg"; // Config file path
    qDebug() << "Project folder path:" << projectPath; // Print the path for debugging
    settings = new QSettings(projectPath, QSettings::IniFormat, this);
}

void SettingsHandler::saveSettings(const QString &username, bool rememberMe) {

    settings->setValue("username", username);
    settings->setValue("rememberMe", rememberMe);
}

QVariantMap SettingsHandler::loadSettings() {

    QVariantMap result;
    result["username"] = settings->value("username", "").toString();
    result["rememberMe"] = settings->value("rememberMe", false).toBool();
    return result;
}

