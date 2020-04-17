cmake_minimum_required(VERSION 3.10)
project(maolan VERSION 0.0.1 DESCRIPTION "Low level Maolan library")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_SHARED_LINKER_FLAGS "-pthread")
file(GLOB SRCS src/*.cpp src/audio/*.cpp src/midi/*.cpp)

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

option(OSS "Open Sound System" ON)
if (OSS)
    file(GLOB OSS_SRCS src/audio/oss/*.cpp)
    set(SRCS ${SRCS} ${OSS_SRCS})
endif()

option(JACK "Jack audio connection kit" ON)
if (JACK)
    pkg_check_modules(JACK REQUIRED jack)
    set(MY_INCLUDE_DIRS ${MY_INCLUDE_DIRS} ${JACK_INCLUDE_DIRS})
    set(MY_LIBRARY_DIRS ${MY_LIBRARY_DIRS} ${JACK_LIBRARY_DIRS})
    set(MY_LIBRARIES ${MY_LIBRARIES} ${JACK_LIBRARIES})
    file(GLOB JACK_SRCS src/audio/jack/*.cpp)
    set(SRCS ${SRCS} ${JACK_SRCS})
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_INSTALL_PREFIX}/include ${MY_INCLUDE_DIRS})
link_directories(${CMAKE_INSTALL_PREFIX}/lib ${MY_LIBRARY_DIRS})

add_library(
    maolan
    SHARED
    ${SRCS}
)
target_link_libraries(maolan ${MY_LIBRARIES})


enable_testing()

add_custom_target(testbuild)

if (OSS)
    add_executable(test-oss EXCLUDE_FROM_ALL tests/oss/cli.cpp)
    target_link_directories(test-oss PUBLIC ${CMAKE_CURRENT_BINARY_DIR})
    target_link_libraries(test-oss maolan)
    add_test(NAME Runs COMMAND ./test-oss http://lv2plug.in/plugins/eg-amp)
    add_dependencies(testbuild test-oss)
endif()
