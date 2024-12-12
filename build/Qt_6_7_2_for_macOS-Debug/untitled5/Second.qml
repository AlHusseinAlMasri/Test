import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: secondPage

    // Receive the `rememberMe` property from `main.qml`
    property bool rememberMe: false

    Column {
        anchors.centerIn: parent

        Text {
            text: rememberMe ? "Remember Me is checked" : "Remember Me is not checked"
            font.pixelSize: 18
        }

        ComboBox {
            id: comboBox
            visible: rememberMe // Only visible if rememberMe is true
            editable: true
            model: ListModel {
                ListElement { text: "Option 1" }
                ListElement { text: "Option 2" }
                ListElement { text: "Option 3" }
            }
        }

        Button {
            text: "Go Back"
            onClicked: stackView.pop()
        }
    }
}
