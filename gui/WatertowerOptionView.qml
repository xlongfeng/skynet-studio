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
                    width: container.width
                    height: container.height
                    Row {
                        anchors.centerIn: parent

                        spacing: 16

                        Image {
                            source: icon
                            sourceSize.width: 320
                            sourceSize.height: 320
                        }

                        Column {
                            spacing: 4

                            Label {
                                text: name
                                font.pointSize: 12
                                leftPadding: 8
                            }

                            LabelSpinBox {
                                text: qsTr("Identity")
                                from: -1
                                to: 127
                                value: identity
                                onValueUpdated: option.setData(option.index(index, 0), value, WatertowerModel.IdentityRole)
                            }

                            LabelSwitch {
                                text: qsTr("On-Off")
                                checked: onOff
                                onValueUpdated: option.setData(option.index(index, 0), checked, WatertowerModel.OnOffRole)
                            }

                            LabelSpinBox {
                                text: qsTr("Radius (cm)")
                                from: 100
                                to: 400
                                value: radius
                                onValueUpdated: option.setData(option.index(index, 0), value, WatertowerModel.RadiusRole)
                            }

                            LabelSpinBox {
                                text: qsTr("Bucket Height (cm)")
                                from: 100
                                to: 500
                                value: bucketHeight
                                onValueUpdated: option.setData(option.index(index, 0), value, WatertowerModel.BucketHeightRole)
                            }

                            LabelSpinBox {
                                text: qsTr("Bucket Quantity")
                                from: 1
                                to: 5
                                value: bucketQuantity
                                onValueUpdated: option.setData(option.index(index, 0), value, WatertowerModel.BucketQuantityRole)
                            }

                            LabelComboBox {
                                text: qsTr("Sensor Type")
                                textRole: "name"
                                model: ListModel {
                                    ListElement { name: "Waterlevel"; value: Options.WaterlevelSensor }
                                    ListElement { name: "Ultrasonic"; value: Options.UltrasonicSensor }
                                }
                                currentIndex: sensorType
                                onValueUpdated: {
                                    onValueModified: option.setData(option.index(index, 0), currentIndex, WatertowerModel.SensorTypeRole)
                                }
                            }

                            LabelSpinBox {
                                text: qsTr("Sensor Quantity")
                                from: 1
                                to: 16
                                value: sensorQuantity
                                onValueUpdated: option.setData(option.index(index, 0), value, WatertowerModel.SensorQuantityRole)
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
