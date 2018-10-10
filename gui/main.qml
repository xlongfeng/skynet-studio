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
import QtQuick.Controls 2.2

import Backend 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 800
    height: 600
    title: qsTr("Skynet Studio")

    header: ToolBar {
        contentHeight: toolButton.implicitHeight

        ToolButton {
            id: toolButton
            text: stackView.depth > 1 ? "\u25C0" : "\u2630"
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            onClicked: {
                if (stackView.depth > 1) {
                    stackView.pop()
                } else {
                    drawer.open()
                }
            }
        }

        Label {
            color: Options.sniffer ? "blue" : "black"
            text: stackView.currentItem.title
            font.pixelSize: Qt.application.font.pixelSize * 1.6
            anchors.centerIn: parent
        }

        Text {
            id: date
            color: Options.sniffer ? "blue" : "black"
            font.pixelSize: Qt.application.font.pixelSize * 1.2
            anchors.right: parent.right
            anchors.rightMargin: 8
            anchors.verticalCenter: parent.verticalCenter

            Timer {
                interval: 500
                running: true
                repeat: true
                onTriggered: date.text = new Date().toLocaleString(Qt.locale(), "yyyy-MM-dd HH:mm:ss")
            }
        }
    }

    Drawer {
        id: drawer
        width: window.width * 0.33
        height: window.height

        Column {
            anchors.fill: parent

            ItemDelegate {
                text: qsTr("Watertower Options")
                width: parent.width
                onClicked: {
                    stackView.push("WatertowerOptionView.qml")
                    drawer.close()
                }
            }
            ItemDelegate {
                text: qsTr("System Options")
                width: parent.width
                onClicked: {
                    stackView.push("SystemOptionView.qml")
                    drawer.close()
                }
            }
        }
    }

    StackView {
        id: stackView
        initialItem: "HomeView.qml"
        anchors.fill: parent
    }
}
