cmake_minimum_required(VERSION 3.27 FATAL_ERROR)

set(ENGINE_PROJECT_NAME Waza3DCore)

add_library(${ENGINE_PROJECT_NAME} STATIC 
	src/test.cpp
	includes/test.hpp
)

target_include_directories(${ENGINE_PROJECT_NAME} PUBLIC includes)
target_compile_features(${ENGINE_PROJECT_NAME} PUBLIC cxx_std_20)

add_subdirectory(../external/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${ENGINE_PROJECT_NAME} PRIVATE glfw)