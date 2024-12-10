#ifndef SETTINGSHANDLER_H
#define SETTINGSHANDLER_H

#include <QObject>
#include <QSettings>

class SettingsHandler : public QObject {
    Q_OBJECT

public:
    explicit SettingsHandler(QObject *parent = nullptr);

    Q_INVOKABLE void saveSettings(const QString &username, bool rememberMe);
    Q_INVOKABLE QVariantMap loadSettings();

private:
    QSettings *settings; // Pointer to QSettings object
};

#endif // SETTINGSHANDLER_H
