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
import QtQuick.Layouts 1.3
import QtQuick 2.0

import Backend 1.0

Item {
    id: control

    property alias icon: image.source
    property var linkStatus
    property alias tunnage: tunnageText.text
    property alias percent: progressBar.value
    property alias timeoutTimes: timeoutTimesText.text
    property alias requestTimes: requestTimesText.text

    Image {
        id: image

        anchors.centerIn: parent

        width: parent.width - 8
        height: parent.height - 8

        opacity: linkStatus === Options.Detached ? 0.5 : 1.0

        sourceSize.width: parent.width - 8
        sourceSize.height: parent.height - 8
    }

    Rectangle {
        anchors.top: image.top
        anchors.left: image.left

        width: 120
        height: 48
        color: Qt.rgba(0.0, 0.0, 0.0, 0.5)
        radius: 5

        visible: linkStatus === Options.Attached

        Text {
            id: tunnageText
            anchors.centerIn: parent
            color: "yellow"
            font.pointSize: 32
        }
    }

    ProgressBar {
        id: progressBar

        anchors.right: image.right
        anchors.rightMargin: 8
        anchors.verticalCenter: image.verticalCenter

        from: 0
        to: 100

        visible: linkStatus === Options.Attached

        background: Rectangle {
            implicitWidth: 24
            implicitHeight: image.height * 0.9
            color: "white"
            radius: 3
        }

        contentItem: Item {
            rotation: 180

            Rectangle {
                anchors.left: parent.left
                anchors.top: parent.top
                anchors.margins: 2
                width: parent.width - 4
                height: progressBar.visualPosition * (parent.height - 4)
                radius: 2

                gradient: Gradient {
                    GradientStop { position: 0.0; color: Qt.rgba(1.0, 0.0, 1.0, 1.0) }
                    GradientStop { position: 1.0; color:
                            Qt.rgba((progressBar.to - progressBar.value) / progressBar.to,
                                    progressBar.value / progressBar.to,
                                    1.0, 1.0) }
                }
            }
        }
    }

    Rectangle {
        anchors.bottom: image.bottom
        anchors.left: image.left
        anchors.right: image.right
        height: 24
        color: Qt.rgba(1.0, 1.0, 1.0, 0.5)
        visible: Options.debug === true

        Row {
            anchors.centerIn: parent

            Text {
                id: timeoutTimesText
                color: "red"
            }

            Text {
                text: " / "
                color: "black"
            }

            Text {
                id: requestTimesText
                color: "blue"
            }
        }
    }

    BusyIndicator {
        anchors.centerIn: parent
        running: linkStatus === Options.Detached
    }
}
