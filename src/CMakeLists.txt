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

enable_testing()

set(STUDIO_SRC
    stdio.c
    utility.c
    settings.cpp
    options.cpp
    daemon.cpp
    watertower.cpp
    watertowermodel.cpp
    halfduplexlinker.cpp
    )

if (UNIX)
    set(STUDIO_SRC ${STUDIO_SRC} os_linux.c)
endif ()

add_library(libstudio STATIC ${STUDIO_SRC})
target_link_libraries(libstudio Qt5::Core Qt5::Quick Qt5::SerialPort)

add_executable(skynet-studio main.cpp)
target_link_libraries(skynet-studio libstudio gui)
install(TARGETS skynet-studio DESTINATION root)
install(FILES S80skynet DESTINATION etc/init.d PERMISSIONS
    OWNER_READ OWNER_WRITE OWNER_EXECUTE
    GROUP_READ GROUP_EXECUTE
    WORLD_READ WORLD_EXECUTE)

add_executable(skynet-sensor main-sensor.cpp sensor.cpp)
target_link_libraries(skynet-sensor libstudio gui)

add_executable(unittest unittest.cpp)
add_test(unittest unittest)
target_link_libraries(unittest libstudio Qt5::Test)
install(TARGETS unittest DESTINATION root)
