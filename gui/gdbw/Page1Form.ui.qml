import QtQuick 2.10
import QtQuick.Controls 2.3
import QtCanvas3D 1.1
import QtQuick.Layouts 1.3

Page {
    id: page
    width: 600
    height: 400
    property alias page: page
    property alias label1: label1
    property alias progressBar: progressBar
    property alias dial: dial
    property alias column1: column1
    property alias button: button
    property alias comboBox: comboBox
    property alias textField: textField
    property alias label: label
    property alias column: column
    property alias row: row

    title: qsTr("Pet")

    Row {
        id: row
        x: 0
        y: 92
        width: 600
        height: 308
        spacing: 30

        Column {
            id: column
            width: 200
            height: 300
            spacing: 10

            Label {
                id: label
                text: qsTr("Enter a name")
            }

            TextField {
                id: textField
            }

            ComboBox {
                id: comboBox
            }

            Button {
                id: button
                text: qsTr("Order")
            }
        }

        Column {
            id: column1
            width: 200
            height: 300
            spacing: 10

            Dial {
                id: dial
                to: 20
                from: 1
                value: 1
            }

            ProgressBar {
                id: progressBar
                width: 180
                to: 20
                from: 1
                value: 1
            }

            Label {
                id: label1
                text: qsTr("1 year old")
            }
        }
    }
}
