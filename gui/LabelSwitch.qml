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

Row {
    property alias text: label.text
    property alias checked: control.checked
    signal valueUpdated

    spacing: 8

    Label {
        id: label
        width: 120
        height: control.implicitHeight
        verticalAlignment: Text.AlignVCenter
    }

    Switch {
        id: control
        onClicked: valueUpdated()
    }
}
