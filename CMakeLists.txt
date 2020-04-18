cmake_minimum_required(VERSION 3.10)
project(maolan VERSION 0.0.1 DESCRIPTION "Low level Maolan library")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_SHARED_LINKER_FLAGS "-pthread")
include(GNUInstallDirs)
file(GLOB SRCS src/*.cpp src/audio/*.cpp src/midi/*.cpp)
file(GLOB HEADERS maolan/*.h)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/maolan)
set(MY_HEADERS ${MY_HEADEARS} ${HEADERS})
file(GLOB HEADERS maolan/audio/*.h)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/maolan/audio)
set(MY_HEADERS ${MY_HEADEARS} ${HEADERS})
file(GLOB HEADERS maolan/midi/*.h)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/maolan/midi)
set(MY_HEADERS ${MY_HEADEARS} ${HEADERS})

enable_testing()
add_custom_target(build-tests)

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
    file(GLOB OSS_HEADERS maolan/audio/oss/*.h)
    set(MY_INCLUDE_DIRS ${MY_INCLUDE_DIRS} maolan/audio/oss)
    set(SRCS ${SRCS} ${OSS_SRCS})
    install(FILES ${OSS_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/maolan/audio/oss)
    set(MY_HEADERS ${MY_HEADEARS} ${OSS_HEADERS})
endif()

option(ALSA "Advance Linux Sound Architecture" ON)
if (ALSA)
    pkg_check_modules(ALSA REQUIRED alsa)
    set(MY_INCLUDE_DIRS ${MY_INCLUDE_DIRS} ${ALSA_INCLUDE_DIRS})
    set(MY_LIBRARY_DIRS ${MY_LIBRARY_DIRS} ${ALSA_LIBRARY_DIRS})
    set(MY_LIBRARIES ${MY_LIBRARIES} ${ALSA_LIBRARIES})
    file(GLOB ALSA_SRCS src/audio/alsa/*.cpp)
    file(GLOB ALSA_HEADERS maolan/audio/alsa/*.h)
    set(SRCS ${SRCS} ${ALSA_SRCS})
    install(FILES ${ALSA_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/maolan/audio/alsa)
    set(MY_HEADERS ${MY_HEADEARS} ${ALSA_HEADERS})
endif()

if (PKG_CONFIG_INSTALL_PATH)
    install(FILES libmaolan.pc DESTINATION ${CMAKE_INSTALL_PREFIX}${PKG_CONFIG_INSTALL_PATH})
elseif (${CMAKE_SYSTEM_NAME} STREQUAL "FreeBSD")
    install(FILES libmaolan.pc DESTINATION ${CMAKE_INSTALL_PREFIX}/libdata/pkgconfig)
else()
    install(FILES libmaolan.pc DESTINATION ${CMAKE_INSTALL_PREFIX}/lib/pkgconfig)
endif()

include_directories(${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_INSTALL_PREFIX}/include ${MY_INCLUDE_DIRS})
link_directories(${CMAKE_INSTALL_PREFIX}/lib ${MY_LIBRARY_DIRS})

add_library(
    maolan
    SHARED
    ${SRCS} ${MY_HEADERS}
)

target_link_libraries(maolan ${MY_LIBRARIES})
install(TARGETS maolan)


if (OSS)
    add_executable(test-oss EXCLUDE_FROM_ALL tests/oss/cli.cpp)
    target_link_directories(test-oss PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${MY_LIBRARY_DIRS})
    target_link_libraries(test-oss maolan ${MY_LIBRARIES})
    add_test(NAME OSS COMMAND ./test-oss http://lv2plug.in/plugins/eg-amp)
    add_dependencies(build-tests test-oss)
endif()

option(ALSA "Advance Linux Sound Architecture" ON)
if (ALSA)
    add_executable(test-alsa EXCLUDE_FROM_ALL tests/alsa/cli.cpp)
    target_link_directories(test-alsa PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${MY_LIBRARY_DIRS})
    target_link_libraries(test-alsa maolan ${MY_LIBRARIES})
    add_test(NAME ALSA COMMAND ./test-alsa http://lv2plug.in/plugins/eg-amp)
    add_dependencies(build-tests test-alsa)
endif()
