cmake_minimum_required(VERSION 3.1)
project(fatpup-ui)

if (NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
    ADD_DEFINITIONS(-DNDEBUG)
endif()

set(CMAKE_CXX_FLAGS_RELEASE "-O3")
set(CMAKE_CXX_STANDARD 11)

find_package(SFML 2.5 COMPONENTS graphics window REQUIRED)

add_subdirectory(fatpup)
include_directories(${FATPUP_INCLUDE_DIR})

add_executable(fatpup-ui board.h board.cpp colors.h movepanel.h movepanel.cpp fatpup-ui.cpp umax4_8.c)
target_link_libraries(fatpup-ui ${FATPUP_LIBRARIES} sfml-graphics sfml-window)
add_custom_command(TARGET fatpup-ui POST_BUILD
                   COMMAND ${CMAKE_COMMAND} -E copy_directory
                   ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:fatpup-ui>/resources)