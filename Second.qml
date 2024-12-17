import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: secondPage
    width: 640
    height: 480

    // Properties to receive settings
    property string username: ""
    property string comboboxValue: ""
    property bool rememberMe: false

    Column {
        anchors.centerIn: parent

        Text {
            text: "Username: " + username
            font.pixelSize: 20
        }

        TextField {
            id: usernameField
            placeholderText: "Enter username"
            text: username // Pre-fill with the value from .cfg
        }

        ComboBox {
            id: comboBox
            editable: true
            model: ListModel {
                ListElement { text: "Banana" }
                ListElement { text: "Apple" }
                ListElement { text: "Coconut" }
            }
            currentText: comboboxValue // Pre-fill with the value from .cfg
        }

        Text {
            text: rememberMe ? "Remember Me is checked" : "Remember Me is not checked"
            font.pixelSize: 20
        }

        Button {
            text: "Go Back"
            onClicked: stackView.pop()
        }
    }
}
