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

import Backend 1.0

Page {
    width: parent.width
    height: parent.height

    title: qsTr("Home")

    GridView {
        id: gridView
        anchors.fill: parent
        cellWidth: width / 3
        cellHeight: height * 0.5

        model: WatertowerModel { }

        delegate: Frame {
            width: gridView.cellWidth
            height: gridView.cellHeight

            Image {
                id: image

                source: icon
                sourceSize.width: parent.width
                sourceSize.height: parent.height

                Rectangle {
                    anchors.top: parent.top
                    anchors.left: parent.left

                    width: 120
                    height: 48
                    color: Qt.rgba(0.0, 0.0, 0.0, 0.5)

                    Text {
                        anchors.centerIn: parent
                        text: tunnage
                        color: "yellow"
                        font.bold: true
                        font.pointSize: 32
                    }
                }

                ProgressBar {
                    id: control

                    anchors.right: parent.right
                    anchors.rightMargin: 8
                    anchors.verticalCenter: parent.verticalCenter

                    from: 0
                    to: 100
                    value: percent

                    background: Rectangle {
                        id: background
                        implicitWidth: 24
                        implicitHeight: image.height * 0.9
                        color: "#e6e6e6"
                        radius: 3
                    }

                    contentItem: Item {
                        rotation: 180

                        Rectangle {
                            width: parent.width
                            height: control.visualPosition * parent.height
                            radius: 2

                            gradient: Gradient {
                                GradientStop { position: 0.0; color: Qt.rgba(1.0, 0.0, 0.0, 1.0) }
                                GradientStop { position: 1.0; color:
                                        Qt.rgba((control.to - control.value) / control.to,
                                                control.value / control.to,
                                                0.0, 1.0) }
                            }
                        }
                    }
                }
            }
        }
    }
}
