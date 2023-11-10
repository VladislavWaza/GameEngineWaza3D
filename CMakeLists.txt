cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(PROJECT_NAME Waza3D)

add_subdirectory(core)
add_subdirectory(editor)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT Waza3DEditor)