import QtQuick 2.9
import QtQuick.Controls 2.2

Row {
    property alias text: label.text
    property alias checked: control.checked
    signal valueUpdated

    spacing: 8

    Label {
        id: label
        width: 120
        height: control.implicitHeight
        verticalAlignment: Text.AlignVCenter
        font.bold: true
    }

    Switch {
        id: control
        onClicked: valueUpdated()
    }
}
