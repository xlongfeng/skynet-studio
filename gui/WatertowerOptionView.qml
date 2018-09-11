import QtQuick 2.9
import QtQuick.Layouts 1.3
import QtQuick.Controls 2.2

import Backend 1.0

Page {
    title: qsTr("Options")

    ListView {
        id: containter
        width: parent.width - 16
        height: parent.height
        anchors.centerIn: parent
        spacing: 16

        model: WatertowerModel { id: option }

        delegate: GroupBox {
            property int row: index

            width: containter.width

            label: Label {
                text: name
                font.pointSize: 12
                leftPadding: 8
            }

            RowLayout {

                spacing: 16

                Image {
                    source: icon
                    sourceSize.width: 240
                    sourceSize.height: 240
                }

                Column {
                    spacing: 4

                    LabelSpinBox {
                        text: qsTr("Identity")
                        from: -1
                        to: 127
                        value: identity
                        onValueUpdated: option.setData(option.index(row, 0), value, WatertowerModel.IdentityRole)
                    }

                    LabelSwitch {
                        text: qsTr("On-Off")
                        checked: onOff
                        onValueUpdated: option.setData(option.index(row, 0), checked, WatertowerModel.OnOffRole)
                    }

                    LabelSpinBox {
                        text: qsTr("Radius (cm)")
                        from: 100
                        to: 400
                        value: radius
                        onValueUpdated: option.setData(option.index(row, 0), value, WatertowerModel.RadiusRole)
                    }

                    LabelSpinBox {
                        text: qsTr("Bucket Quantity")
                        from: 1
                        to: 5
                        value: bucketQuantity
                        onValueUpdated: option.setData(option.index(row, 0), value, WatertowerModel.BucketQuantityRole)
                    }

                    LabelComboBox {
                        text: qsTr("Sensor Type")
                        textRole: "name"
                        model: ListModel {
                            ListElement { name: "Waterlevel"; value: Watertower.WaterlevelSensor }
                            ListElement { name: "Ultrasonic"; value: Watertower.UltrasonicSensor }
                        }
                        currentIndex: sensorType
                        onValueUpdated: {
                            onValueModified: option.setData(option.index(row, 0), currentIndex, WatertowerModel.SensorTypeRole)
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
