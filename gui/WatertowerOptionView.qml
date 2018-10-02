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
    title: qsTr("Watertower Options")

    SwipeView {
        id: container

        anchors.fill: parent

        Repeater {
            model: WatertowerModel { id: option }

            Loader {
                active: SwipeView.isCurrentItem || SwipeView.isNextItem || SwipeView.isPreviousItem
                sourceComponent: Item {
                    property int indexRow: index
                    width: container.width
                    height: container.height
                    Column {
                        anchors.centerIn: parent
                        spacing: 8

                        RowLayout {
                            width: control.width
                            Label {
                                text: name
                                font.pointSize: 24
                                Layout.leftMargin: 8
                                Layout.fillWidth: true
                            }

                            Switch {
                                checked: onOff
                                onClicked: option.setData(option.index(indexRow, 0), checked, WatertowerModel.OnOffRole)
                            }
                        }

                        Row {
                            id: control

                            spacing: 8

                            Watertower {
                                width: 320
                                height: 320
                                icon: model.icon
                                linkStatus: model.linkStatus
                                tunnage: model.tunnage
                                percent: model.percent
                                requestTimes: model.requestTimes
                                timeoutTimes: model.timeoutTimes
                            }

                            Grid {
                                columns: 2
                                spacing: 4

                                Label {
                                    text: qsTr("Identity")
                                    font.pointSize: 12
                                }

                                TextField {
                                    text: identity
                                    readOnly: true
                                    width: 160
                                }

                                Label {
                                    text: qsTr("Bucket Radius (cm)")
                                    font.pointSize: 12
                                }

                                SpinBox {
                                    from: 30
                                    to: 300
                                    value: radius
                                    width: 150
                                    onValueModified: option.setData(option.index(indexRow, 0), value, WatertowerModel.RadiusRole)
                                }

                                Label {
                                    text: qsTr("Bucket Height (cm)")
                                    font.pointSize: 12
                                }

                                SpinBox {
                                    from: 100
                                    to: 500
                                    value: bucketHeight
                                    width: 160
                                    onValueModified: option.setData(option.index(indexRow, 0), value, WatertowerModel.BucketHeightRole)
                                }

                                Label {
                                    text: qsTr("Bucket Quantity")
                                    font.pointSize: 12
                                }

                                SpinBox {
                                    from: 1
                                    to: 5
                                    value: bucketQuantity
                                    width: 160
                                    onValueModified: option.setData(option.index(indexRow, 0), value, WatertowerModel.BucketQuantityRole)
                                }

                                Label {
                                    text: qsTr("Sensor Type")
                                    font.pointSize: 12
                                }

                                ComboBox {
                                    textRole: "name"
                                    model: ListModel {
                                        ListElement { name: qsTr("Unknown"); value: Options.UnknownSensor }
                                        ListElement { name: qsTr("Waterlevel"); value: Options.WaterlevelSensor }
                                        ListElement { name: qsTr("Ultrasonic"); value: Options.UltrasonicSensor }
                                    }
                                    width: 160
                                    onActivated: option.setData(option.index(indexRow, 0), model.get(index).value, WatertowerModel.SensorTypeRole)
                                    Component.onCompleted: {
                                        currentIndex = -1;
                                        var count = model.count
                                        for (var i = 0; i < count; i++) {
                                            if (model.get(i).value === sensorType) {
                                                currentIndex = i;
                                                break;
                                            }
                                        }
                                    }
                                }

                                Label {
                                    text: qsTr("Sensor Quantity")
                                    font.pointSize: 12
                                }

                                SpinBox {
                                    from: 1
                                    to: 16
                                    value: sensorQuantity
                                    width: 160
                                    onValueModified: option.setData(option.index(indexRow, 0), value, WatertowerModel.SensorQuantityRole)
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    PageIndicator {
        id: indicator

        count: container.count
        currentIndex: container.currentIndex

        anchors.bottom: container.bottom
        anchors.horizontalCenter: parent.horizontalCenter
    }
}
