cmake_minimum_required(VERSION 3.16)

project(sdl_hello)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
add_executable(sdl_hello hello.cpp)
target_link_libraries(sdl_hello SDL2)

install(TARGETS sdl_hello
	DESTINATION ${PROJECT_SOURCE_DIR})
