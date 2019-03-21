import QtQuick 2.0
import QtQuick.Dialogs 1.2

Page1Form {

    comboBox.model: ["Dog", "Cat", "Bird", "Fish"]

    button.onClicked: {
        var order = "Ordering: " + textField.text + "\r\n"
        order = order + "Your " + Math.round(dial.value) + " year old " + comboBox.currentText

        messageDialog.text = order
        messageDialog.open()

    }

    dial.onValueChanged: {
        progressBar.value = dial.value
        label1.text = Math.round(dial.value) + "Years Old"
    }

    MessageDialog {
        id: messageDialog
        title: "Hello world"
        text: ""
        onAccepted: Qt.quit()

    }

}
