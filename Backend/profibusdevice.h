// ProfibusDevice.h
#ifndef PROFIBUSDEVICE_H
#define PROFIBUSDEVICE_H

#include <QObject>
#include <QString>
#include <QByteArray>

// Profibus-Bibliothek einbinden
#include "prof_dp.h"

class ProfibusDevice : public QObject {
    Q_OBJECT
public:
    explicit ProfibusDevice(QObject *parent = nullptr);
    ~ProfibusDevice();

    Q_INVOKABLE bool initialize(int stationAddress);    // Initialisierung des Masters
    Q_INVOKABLE void close();                           // Verbindung schließen
    Q_INVOKABLE QByteArray readData(int slaveAddress);  // Daten von einem Slave lesen
    Q_INVOKABLE bool writeData(int slaveAddress, const QByteArray &data); // Daten an einen Slave schreiben

    Q_PROPERTY(type name READ name WRITE setName NOTIFY nameChanged FINAL)
signals:
    void errorOccurred(const QString &error);
    void dataReceived(int slaveAddress, const QByteArray &data);

private:
    bool isInitialized; // Status der Initialisierung
};

#endif // PROFIBUSDEVICE_H
