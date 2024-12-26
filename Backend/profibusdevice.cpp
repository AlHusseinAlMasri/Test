// ProfibusDevice.cpp
#include "ProfibusDevice.h"

ProfibusDevice::ProfibusDevice(QObject *parent) : QObject(parent), isInitialized(false) {}

ProfibusDevice::~ProfibusDevice() {
    if (isInitialized) {
        close();
    }
}

bool ProfibusDevice::initialize(int stationAddress) {
    // Beispiel: Profibus initialisieren
    int result = DPInit(stationAddress);  // Funktion aus prof_dp.h
    if (result == 0) {
        isInitialized = true;
        return true;
    } else {
        emit errorOccurred("Failed to initialize Profibus: Error code " + QString::number(result));
        return false;
    }
}

void ProfibusDevice::close() {
    if (isInitialized) {
        prof_dp_close();  // Verbindung schließen
        isInitialized = false;
    }
}

QByteArray ProfibusDevice::readData(int slaveAddress) {
    if (!isInitialized) {
        emit errorOccurred("Profibus not initialized!");
        return QByteArray();
    }

    unsigned char buffer[256];  // Beispiel: Buffergröße
    int bytesRead = prof_dp_read(slaveAddress, buffer, sizeof(buffer));
    if (bytesRead < 0) {
        emit errorOccurred("Failed to read data from slave " + QString::number(slaveAddress));
        return QByteArray();
    }

    return QByteArray(reinterpret_cast<char*>(buffer), bytesRead);
}

bool ProfibusDevice::writeData(int slaveAddress, const QByteArray &data) {
    if (!isInitialized) {
        emit errorOccurred("Profibus not initialized!");
        return false;
    }

    int result = prof_dp_write(slaveAddress, reinterpret_cast<const unsigned char*>(data.data()), data.size());
    if (result != 0) {
        emit errorOccurred("Failed to write data to slave " + QString::number(slaveAddress));
        return false;
    }

    return true;
}
