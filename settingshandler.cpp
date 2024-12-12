#include "settingshandler.h"
#include <QDir>
#include <QMessageBox>

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
    // Show a confirmation message box
    QMessageBox msgBox;
    msgBox.setText("Are you sure you want to save the settings?");
    msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
    msgBox.setDefaultButton(QMessageBox::No);

    int ret = msgBox.exec(); // Show the message box and wait for user input

    if (ret == QMessageBox::Yes) {
        settings->beginGroup("DATA XY");
        settings->setValue("username", username);
        settings->setValue("rememberMe", rememberMe);
    }


}

QVariantMap SettingsHandler::loadSettings() {

    QVariantMap result;
    result["username"] = settings->value("username", "").toString();
    result["rememberMe"] = settings->value("rememberMe", false).toBool();
    return result;
}

