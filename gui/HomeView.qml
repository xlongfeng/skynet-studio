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

        delegate: Watertower {
            width: gridView.cellWidth
            height: gridView.cellHeight
            icon: model.icon
            linkStatus: model.linkStatus
            tunnage: model.tunnage
            percent: model.percent
        }
    }
}
