cmake_minimum_required(VERSION 3.10)
project(maolan VERSION 0.0.1 DESCRIPTION "Low level Maolan library")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_SHARED_LINKER_FLAGS "-pthread")
include(GNUInstallDirs)

file(GLOB SRCS src/*.cpp src/audio/*.cpp src/midi/*.cpp src/plugin/*.cpp)
file(GLOB HEADERS maolan/*.hpp)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/maolan)
set(MY_HEADERS ${MY_HEADEARS} ${HEADERS})
file(GLOB HEADERS maolan/audio/*.hpp)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/maolan/audio)
set(MY_HEADERS ${MY_HEADEARS} ${HEADERS})
file(GLOB HEADERS maolan/midi/*.hpp)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/maolan/midi)
set(MY_HEADERS ${MY_HEADEARS} ${HEADERS})
file(GLOB HEADERS maolan/plugin/*.hpp)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/maolan/plugin)
set(MY_HEADERS ${MY_HEADEARS} ${HEADERS})
file(GLOB HEADERS maolan/plugin/lv2/*.hpp)
install(FILES ${HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/maolan/plugin/lv2)
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

option(OSS "Open Sound System" OFF)
if (OSS)
    file(GLOB OSS_AUDIO_SRCS src/audio/oss/*.cpp)
    file(GLOB OSS_MIDI_SRCS src/midi/oss/*.cpp)
    file(GLOB OSS_AUDIO_HEADERS maolan/audio/oss/*.hpp)
    file(GLOB OSS_MIDI_HEADERS maolan/midi/oss/*.hpp)
    set(MY_INCLUDE_DIRS ${MY_INCLUDE_DIRS} maolan/audio/oss maolan/midi/oss)
    set(SRCS ${SRCS} ${OSS_AUDIO_SRCS} ${OSS_MIDI_SRCS})
    install(FILES ${OSS_AUDIO_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/maolan/audio/oss)
    install(FILES ${OSS_MIDI_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/maolan/midi/oss)
    set(MY_HEADERS ${MY_HEADEARS} ${OSS_AUDIO_HEADERS} ${OSS_MIDI_HEADERS})

    add_executable(test-oss EXCLUDE_FROM_ALL tests/oss/cli.cpp)
    target_link_directories(test-oss PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${MY_LIBRARY_DIRS})
    target_link_libraries(test-oss maolan ${MY_LIBRARIES})
    add_dependencies(build-tests test-oss)
    add_test(NAME oss COMMAND ./test-oss http://lv2plug.in/plugins/eg-amp)

    add_executable(test-oss-midi EXCLUDE_FROM_ALL tests/oss/midi.cpp)
    target_link_directories(test-oss-midi PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${MY_LIBRARY_DIRS})
    target_link_libraries(test-oss-midi maolan ${MY_LIBRARIES})
    add_dependencies(build-tests test-oss-midi)
    add_test(NAME oss-midi COMMAND ./test-oss-midi /dev/umidi)
endif()

option(ALSA "Advance Linux Sound Architecture" OFF)
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

    add_executable(test-alsa EXCLUDE_FROM_ALL tests/alsa/cli.cpp)
    target_link_directories(test-alsa PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${MY_LIBRARY_DIRS})
    target_link_libraries(test-alsa maolan ${MY_LIBRARIES})
    add_dependencies(build-tests test-alsa)
    add_test(NAME alsa COMMAND ./test-alsa http://lv2plug.in/plugins/eg-amp)
endif()

option(LV2 "LADSPA version 2 plugins" ON)
if (LV2)
    pkg_check_modules(LILV REQUIRED lilv-0)
    set(MY_INCLUDE_DIRS ${MY_INCLUDE_DIRS} ${LILV_INCLUDE_DIRS})
    set(MY_LIBRARY_DIRS ${MY_LIBRARY_DIRS} ${LILV_LIBRARY_DIRS})
    set(MY_LIBRARIES ${MY_LIBRARIES} ${LILV_LIBRARIES})

    file(GLOB LV2_SRCS src/plugin/lv2/*.cpp)
    file(GLOB LV2_HEADERS maolan/plugin/lv2/*.h)
    set(MY_INCLUDE_DIRS ${MY_INCLUDE_DIRS} maolan/plugin/lv2)
    set(SRCS ${SRCS} ${LV2_SRCS})
    install(FILES ${LV2_HEADERS} DESTINATION ${CMAKE_INSTALL_PREFIX}/include/maolan/plugin/lv2)
    set(MY_HEADERS ${MY_HEADEARS} ${LV2_HEADERS})

    add_executable(test-lv2 EXCLUDE_FROM_ALL tests/lv2/cli.cpp)
    target_link_directories(test-lv2 PUBLIC ${CMAKE_CURRENT_BINARY_DIR} ${MY_LIBRARY_DIRS})
    target_link_libraries(test-lv2 maolan ${MY_LIBRARIES})
    add_dependencies(build-tests test-lv2)
    add_test(NAME lv2 COMMAND ./test-lv2)
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


option(BUILD_SHARED "Build shared library (libmaolan.so)" ON)
if (BUILD_SHARED)
    add_library(
        maolan
        SHARED
        ${SRCS} ${MY_HEADERS}
    )
    add_dependencies(build-tests maolan)
    target_link_libraries(maolan ${MY_LIBRARIES})
    install(TARGETS maolan)
endif()

option(BUILD_STATIC "Build static library (libmaolan.a)" OFF)
if (BUILD_STATIC)
    add_library(
        maolan-static
        STATIC
        ${SRCS} ${MY_HEADERS}
    )
    add_dependencies(build-tests maolan-static)
    if (NOT ${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
        set_target_properties(maolan-static PROPERTIES OUTPUT_NAME maolan)
    endif()
    target_link_libraries(maolan-static ${MY_LIBRARIES})
    install(TARGETS maolan-static)
endif()


if (NOT BUILD_SHARED AND NOT BUILD_STATIC)
    message(FATAL_ERROR "You have to build at least one: SHARED or STATIC library")
endif()
