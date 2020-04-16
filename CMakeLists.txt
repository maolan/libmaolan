cmake_minimum_required(VERSION 3.10)
project(maolan VERSION 0.0.1 DESCRIPTION "Low level Maolan library")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_SHARED_LINKER_FLAGS "-pthread")

find_package(PkgConfig REQUIRED)

pkg_check_modules(SNDFILE REQUIRED sndfile)
set(MY_INCLUDE_DIRS ${MY_INCLUDE_DIRS} ${SNDFILE_INCLUDE_DIRS})
set(MY_LIBRARY_DIRS ${MY_LIBRARY_DIRS} ${SNDFILE_LIBRARY_DIRS})
set(MY_LIBRARIES ${MY_LIBRARIES} ${SNDFILE_LIBRARIES})

pkg_check_modules(PUGIXML REQUIRED pugixml)
set(MY_INCLUDE_DIRS ${MY_INCLUDE_DIRS} ${PUGIXML_INCLUDE_DIRS})
set(MY_LIBRARY_DIRS ${MY_LIBRARY_DIRS} ${PUGIXML_LIBRARY_DIRS})
set(MY_LIBRARIES ${MY_LIBRARIES} ${PUGIXML_LIBRARIES})

pkg_check_modules(LILV REQUIRED lilv-0)
set(MY_INCLUDE_DIRS ${MY_INCLUDE_DIRS} ${LILV_INCLUDE_DIRS})
set(MY_LIBRARY_DIRS ${MY_LIBRARY_DIRS} ${LILV_LIBRARY_DIRS})
set(MY_LIBRARIES ${MY_LIBRARIES} ${LILV_LIBRARIES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_INSTALL_PREFIX}/include ${MY_INCLUDE_DIRS})
link_directories(${CMAKE_INSTALL_PREFIX}/lib ${MY_LIBRARY_DIRS})

file(GLOB SRCS src/*.cpp src/audio/*.cpp src/midi/*.cpp)
add_library(
    maolan
    SHARED
    ${SRCS}
)
target_link_libraries(maolan ${MY_LIBRARIES})

set(MAOLANCLI_COMPILE_COMMAND ${CMAKE_CXX_COMPILER} -I${CMAKE_CURRENT_SOURCE_DIR} -std=gnu++${CMAKE_CXX_STANDARD} ${SNDFILE_CFLAGS} ${PUGIXML_CFLAGS} ${LILV_CFLAGS} ${SNDFLE_LDFLAGS} ${PUGIXML_LDFLAGS} ${LILV_LDFLAGS} -L${CMAKE_CURRENT_BINARY_DIR} -lmaolan ../maolancli.cpp -o maolancli)
set(MAOLANCLI_DEBUG_LLDB_COMMAND env LD_PRELOAD=./libmaolan.so lldb ./maolancli -- http://lv2plug.in/plugins/eg-amp)
set(MAOLANCLI_DEBUG_GDB_COMMAND env LD_PRELOAD=./libmaolan.so gdb ./maolancli http://lv2plug.in/plugins/eg-amp)
add_custom_target(
    maolancli
    COMMAND ${MAOLANCLI_COMPILE_COMMAND}
    COMMAND env LD_PRELOAD=./libmaolan.so ./maolancli http://lv2plug.in/plugins/eg-amp
)
add_custom_target(
    maolancli-debug
    COMMAND ${MAOLANCLI_COMPILE_COMMAND} -g
    COMMAND  env LD_PRELOAD=./libmaolan.so lldb ./maolancli -- http://lv2plug.in/plugins/eg-amp
)
add_custom_target(
    maolancli-debug-gdb
    COMMAND ${MAOLANCLI_COMPILE_COMMAND} -g
    COMMAND  env LD_PRELOAD=./libmaolan.so gdb --args ./maolancli http://lv2plug.in/plugins/eg-amp
)
