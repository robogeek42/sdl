cmake_minimum_required(VERSION 3.16)

project(sdl_sprite_test)
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})
add_executable(sdl_sprite_test sdl_sprite_test.cpp MySprite.cpp)
target_link_libraries(sdl_sprite_test SDL2 SDL2_image SDL2_ttf SDL2_gfx)

install(TARGETS sdl_sprite_test
	DESTINATION ${PROJECT_SOURCE_DIR})
