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
    }

    Dialog {
        id: shutdownDialog
        modal: true
        title: "Power Saving Time"
        x: parent.width / 2 - width / 2
        y: parent.height / 2 - height / 2
        standardButtons: Dialog.Ok | Dialog.Cancel
        GridLayout {
            columns: 2
            Text {
                text: qsTr("From")
                Layout.fillWidth: true
                Layout.columnSpan: 2
            }
            Text {
                text: qsTr("Hour")
                Layout.preferredWidth: 120
            }
            SpinBox {
                id: powerSavingFromHour
                from: 0
                to: 23
                value: Options.powerSavingFrom.getHours()
            }
            Text {
                text: qsTr("Minute")
                Layout.preferredWidth: 120
            }
            SpinBox {
                id: powerSavingFromMinute
                from: 0
                to: 59
                value: Options.powerSavingFrom.getMinutes()
            }
            Rectangle {
                color: "gray"
                Layout.preferredHeight: 1
                Layout.fillWidth: true
                Layout.columnSpan: 2
            }
            Text {
                text: qsTr("To")
                Layout.fillWidth: true
                Layout.columnSpan: 2
            }
            Text {
                text: qsTr("Hour")
                Layout.preferredWidth: 120
            }
            SpinBox {
                id: powerSavingToHour
                from: 0
                to: 23
                value: Options.powerSavingTo.getHours()
            }
            Text {
                text: qsTr("Minute")
                Layout.preferredWidth: 120
            }
            SpinBox {
                id: powerSavingToMinute
                from: 0
                to: 59
                value: Options.powerSavingTo.getMinutes()
            }
        }
        onAccepted: {
            var from = Options.powerSavingFrom
            var to = Options.powerSavingTo
            from.setHours(powerSavingFromHour.value)
            from.setMinutes(powerSavingFromMinute.value)
            to.setHours(powerSavingToHour.value)
            to.setMinutes(powerSavingToMinute.value)
            Options.powerSavingFrom = from
            Options.powerSavingTo = to
        }
    }

    GridLayout {
        columns: 2
        anchors.centerIn: parent

        Button {
            text: qsTr("Date and Time")
            onPressed: {
                var date = new Date()
                year.value = date.getFullYear()
                month.value = date.getMonth() + 1
                day.value = date.getDate()
                hour.value = date.getHours()
                minute.value = date.getMinutes()
                second.value = date.getSeconds()
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
            from: Options.backlightMin()
            to: Options.backlightMax()
            value: Options.backlight
            stepSize: 1
            onMoved: Options.backlight = value
            Layout.preferredWidth: 160
        }

        Text {
            text: qsTr("Volume")
        }

        Slider {
            Layout.preferredWidth: 160
        }

        Rectangle {
            color: "gray"
            Layout.preferredHeight: 1
            Layout.fillWidth: true
            Layout.columnSpan: 2
        }

        CheckBox {
            text: qsTr("Auto Shutdown")
            checked: Options.autoShutdown
            onClicked: Options.autoShutdown = checked
            Layout.columnSpan: 2
            Layout.alignment: Qt.AlignRight
        }

        Text {
            text: qsTr("Power Saving Time")
        }

        TextField {
            enabled: Options.autoShutdown
            text: Options.powerSavingFrom.toLocaleTimeString(Qt.locale(), "hh:mm") + " ~ " +
                  Options.powerSavingTo.toLocaleTimeString(Qt.locale(), "hh:mm")
            readOnly: true
            onPressAndHold: shutdownDialog.open()
            Layout.preferredWidth: 160
        }

        Text {
            text: qsTr("Idle Time")
        }

        ComboBox {
            textRole: "name"
            model: ListModel {
                ListElement { name: "5 min"; value: 5}
                ListElement { name: "10 min"; value: 10}
                ListElement { name: "15 min"; value: 15}
                ListElement { name: "20 min"; value: 20}
                ListElement { name: "30 min"; value: 30}
            }
            enabled: Options.autoShutdown
            onActivated: {
                Options.idleTime = model.get(index).value
            }
            Component.onCompleted: {
                currentIndex = -1;
                var count = model.count
                for (var i = 0; i < count; i++) {
                    if (model.get(i).value === Options.idleTime) {
                        currentIndex = i;
                        break;
                    }
                }
            }

            Layout.preferredWidth: 160
        }
    }
}
