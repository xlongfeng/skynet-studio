set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR arm)

set(CMAKE_C_COMPILER /home/skynet/buildroot/output/host/usr/bin/arm-linux-gcc)
set(CMAKE_CXX_COMPILER /home/skynet/buildroot/output/host/usr/bin/arm-linux-g++)

set(CMAKE_SYSROOT /home/skynet/buildroot/output/staging)
# set(CMAKE_STAGING_PREFIX /home/skynet/buildroot/output/staging)

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)
