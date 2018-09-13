import QtQuick 2.9
import QtQuick.Controls 2.2

Row {
    property alias text: label.text
    property alias from: control.from
    property alias to: control.to
    property alias value: control.value
    signal valueUpdated

    spacing: 8

    Label {
        id: label
        width: 120
        height: control.implicitHeight
        verticalAlignment: Text.AlignVCenter
    }

    SpinBox {
        id: control
        onValueModified: valueUpdated()
    }
}
