import QtQuick 2.15
import QtQuick.Controls 2.15
import com.example.profibus 1.0

ApplicationWindow {
    visible: true
    width: 800
    height: 600
    title: "Profibus Controller"

    ProfibusDevice {
        id: profibus
        onErrorOccurred: console.log("Error: " + error)
        onDataReceived: console.log("Data received from slave " + slaveAddress + ": " + data)
    }

    Column {
        anchors.centerIn: parent
        spacing: 10

        Button {
            text: "Initialize"
            onClicked: profibus.initialize(1)  // Station Address 1
        }

        Button {
            text: "Read Data"
            onClicked: {
                var data = profibus.readData(10)  // Slave Address 10
                console.log("Read Data: " + data)
            }
        }

        Button {
            text: "Write Data"
            onClicked: profibus.writeData(10, "Hello Slave")
        }

        Button {
            text: "Close"
            onClicked: profibus.close()
        }
    }
}
