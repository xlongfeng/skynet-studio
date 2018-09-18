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

ApplicationWindow {
    id: window
    visible: true
    width: 640
    height: 480
    title: qsTr("Skynet Sensor")

    GridLayout {
        columns: 4
        anchors.centerIn: parent

        Text {
            text: qsTr("Serial Port")
            Layout.columnSpan: 2
        }

        ComboBox {
            id: portName
            model: Options.availablePorts()
            onActivated: Options.portName = portName.textAt(index)
            Component.onCompleted: {
                portName.currentIndex = portName.find(Options.portName)
            }
            Layout.fillWidth: true
            Layout.columnSpan: 2
        }

        Text {
            text: qsTr("Baud Rate")
            Layout.columnSpan: 2
        }

        ComboBox {
            id: baudRate
            model: Options.standardBaudRates()
            onActivated: Options.baudRate = baudRate.textAt(index)
            Component.onCompleted: {
                baudRate.currentIndex = baudRate.find(Options.baudRate)
            }
            Layout.fillWidth: true
            Layout.columnSpan: 2
        }

        Text {
            text: qsTr("Device ID")
        }

        SpinBox {
            id: deviceID
            from: 0x00
            to: 0x3f
        }

        Button {
            text: qsTr("Get")
            onClicked: Sensor.getIdentity()
        }

        Button {
            text: qsTr("Set")
            onClicked: Sensor.identity = deviceID.value
        }

        Text {
            text: qsTr("Device Type")
        }

        ComboBox {
            id: deviceType
            textRole: "name"
            model: ListModel {
                ListElement { name: qsTr("Unknown"); value: Options.UnknownSensor }
                ListElement { name: qsTr("Waterlevel"); value: Options.WaterlevelSensor }
                ListElement { name: qsTr("Ultrasonic"); value: Options.UltrasonicSensor }
            }
            currentIndex: 0
        }

        Button {
            text: qsTr("Get")
            onClicked: Sensor.getSensorType()
        }

        Button {
            text: qsTr("Set")
            onClicked: Sensor.sensorType = deviceType.currentIndex
        }

        TextField {
            placeholderText: qsTr("Sensor Measure Result")
            readOnly: true
            Layout.fillWidth: true
            Layout.columnSpan: 2
        }

        Button {
            text: qsTr("Query")
            onClicked: Sensor.queryDevice()
            Layout.fillWidth: true
            Layout.columnSpan: 2
        }

        Button {
            text: qsTr("Linker Stability Test")
            checkable: true
            onClicked: {
                if (checked) {
                    Sensor.testStart()
                } else {
                    Sensor.testStop()
                }
            }

            Layout.fillWidth: true
            Layout.columnSpan: 4
        }

        Text {
            text: qsTr("Request Times")
            Layout.columnSpan: 2
        }

        TextField {
            text: Sensor.requestTimes
            readOnly: true
            Layout.fillWidth: true
            Layout.columnSpan: 2
        }

        Text {
            text: qsTr("Response Timeout Times")
            Layout.columnSpan: 2
        }

        TextField {
            text: Sensor.responseTimeoutTimes
            readOnly: true
            Layout.fillWidth: true
            Layout.columnSpan: 2
        }

        Text {
            text: qsTr("Response Latency")
            Layout.columnSpan: 2
        }

        TextField {
            text: Sensor.responseLatency + " ms"
            readOnly: true
            Layout.fillWidth: true
            Layout.columnSpan: 2
        }
    }
}
