import QtQuick 2.9
import QtQuick.Controls 2.2

Row {
    property alias text: label.text
    property alias textRole: control.textRole
    property alias model: control.model
    property alias currentIndex: control.currentIndex
    signal valueUpdated

    spacing: 8

    Label {
        id: label
        width: 120
        height: control.implicitHeight
        verticalAlignment: Text.AlignVCenter
    }

    ComboBox {
        id: control
        onActivated: valueUpdated()
    }
}
