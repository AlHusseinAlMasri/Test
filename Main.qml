import QtQuick 2.15
import QtQuick.Controls 2.15

ApplicationWindow {
    visible: true
    width: 400
    height: 300
    title: "Settings Example"

    Column {
        anchors.centerIn: parent
        spacing: 20

        TextField {
            id: usernameField
            placeholderText: "Enter your username"
            text: ""
        }

        CheckBox {
            id: rememberMeCheckbox
            text: "Remember Me"
            checked: false
        }

        Button {
            text: "Save Settings"
            onClicked: {
                settingsHandler.saveSettings(usernameField.text, rememberMeCheckbox.checked)
            }
        }

        Button {
            text: "Load Settings"
            onClicked: {
                var settings = settingsHandler.loadSettings()
                usernameField.text = settings.username
                rememberMeCheckbox.checked = settings.rememberMe
            }
        }
    }
}
