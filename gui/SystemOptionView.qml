/*
 * skynet studio
 *
 * Copyright (C) 2018 xlongfeng <xlongfeng@126.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import Backend 1.0

Page {
    title: qsTr("System Options")

    Dialog {
        id: dateTimeDialog
        modal: true
        title: "Date and Time"
        x: parent.width / 2 - width / 2
        y: parent.height / 2 - height / 2
        standardButtons: Dialog.Ok | Dialog.Cancel
        GridLayout {
            columns: 2
            Text {
                text: qsTr("Year")
                Layout.preferredWidth: 120
            }
            SpinBox {
                id: year
                from: 2010
                to: 2200
            }
            Text {
                text: qsTr("Month")
                Layout.preferredWidth: 120
            }
            SpinBox {
                id: month
                from: 1
                to: 12
            }
            Text {
                text: qsTr("Day")
                Layout.preferredWidth: 120
            }
            SpinBox {
                id: day
                from: 1
                to: 31
            }
            Text {
                text: qsTr("Hour")
                Layout.preferredWidth: 120
            }
            SpinBox {
                id: hour
                from: 0
                to: 23
            }
            Text {
                text: qsTr("Minute")
                Layout.preferredWidth: 120
            }
            SpinBox {
                id: minute
                from: 0
                to: 59
            }
            Text {
                text: qsTr("Second")
                Layout.preferredWidth: 120
            }
            SpinBox {
                id: second
                from: 0
                to: 59
            }
        }
        onAccepted: Options.setHardwareClock(year.value, month.value, day.value,
                                             hour.value, minute.value, second.value)

        Component.onCompleted: {
            var date = new Date()
            year.value = date.getFullYear()
            month.value = date.getMonth() + 1
            day.value = date.getDate()
            hour.value = date.getHours()
            minute.value = date.getMinutes()
            second.value = date.getSeconds()
        }
    }

    Dialog {
        id: shutdownDialog
        modal: true
        title: "Shutdown Time"
        x: parent.width / 2 - width / 2
        y: parent.height / 2 - height / 2
        standardButtons: Dialog.Ok | Dialog.Cancel
        GridLayout {
            columns: 2
            Text {
                text: qsTr("Hour")
                Layout.preferredWidth: 120
            }
            SpinBox {
                id: shutdownHour
                from: 0
                to: 23
                value: Options.shutdownHour
            }
            Text {
                text: qsTr("Minute")
                Layout.preferredWidth: 120
            }
            SpinBox {
                id: shutdownMinute
                from: 0
                to: 59
                value: Options.shutdownMinute
            }
        }
        onAccepted: {
            Options.shutdownHour = shutdownHour.value
            Options.shutdownMinute = shutdownMinute.value
        }
    }

    GridLayout {
        columns: 2
        anchors.centerIn: parent

        Button {
            text: qsTr("Date and Time")
            onPressed: {
                dateTimeDialog.open()
            }
            Layout.columnSpan: 2
            Layout.fillWidth: true
        }

        Text {
            text: qsTr("Serial Port")
            Layout.preferredWidth: 120
        }

        ComboBox {
            id: portName
            model: Options.availablePorts()
            onActivated: Options.portName = portName.textAt(index)
            Component.onCompleted: {
                portName.currentIndex = portName.find(Options.portName)
            }
            Layout.preferredWidth: 160
        }

        Text {
            text: qsTr("Baud Rate")
        }

        ComboBox {
            id: baudRate
            model: Options.standardBaudRates()
            onActivated: Options.baudRate = baudRate.textAt(index)
            Component.onCompleted: {
                baudRate.currentIndex = baudRate.find(Options.baudRate)
            }
            Layout.preferredWidth: 160
        }

        Text {
            text: qsTr("Brightness")
        }

        Slider {
            Layout.preferredWidth: 160
        }

        Text {
            text: qsTr("Volume")
        }

        Slider {
            Layout.preferredWidth: 160
        }

        CheckBox {
            text: qsTr("Auto Shutdown")
            checked: Options.autoShutdown
            onClicked: Options.autoShutdown = checked
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignRight
        }

        Text {
            text: qsTr("Shutdown Time")
        }

        TextField {
            enabled: Options.autoShutdown
            text: "0%1".arg(Options.shutdownHour).slice(-2) + ":" + "0%1".arg(Options.shutdownMinute).slice(-2)
            readOnly: true
            onPressAndHold: shutdownDialog.open()
            Layout.preferredWidth: 160
        }

        Text {
            text: qsTr("Idle Time")
        }

        SpinBox {
            from: 5
            to: 60
            enabled: Options.autoShutdown
            Layout.preferredWidth: 160
        }
    }
}
