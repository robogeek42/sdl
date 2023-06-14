cmake_minimum_required(VERSION 3.16)

project(sdl_renderer_hello)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
add_executable(hello hello.cpp)
target_link_libraries(hello SDL2 SDL2_image SDL2_ttf)

install(TARGETS hello
	DESTINATION ${PROJECT_SOURCE_DIR})
