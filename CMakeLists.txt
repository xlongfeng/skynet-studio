#############################################################################
#
# skynet studio
#
# Copyright (C) 2018 xlongfeng <xlongfeng@126.com>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.
#
#############################################################################

cmake_minimum_required(VERSION 2.8.12)

project(skynet-studio)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 COMPONENTS Core Test Quick SerialPort REQUIRED)

if (CMAKE_CROSSCOMPILING)
    set(IMAGES_PATH /root/images)
else ()
    set(IMAGES_PATH ${PROJECT_SOURCE_DIR}/images)
endif ()

install(DIRECTORY images DESTINATION root)

add_definitions(-DIMAGES_PATH="${IMAGES_PATH}")

add_subdirectory(gui)
add_subdirectory(src)
