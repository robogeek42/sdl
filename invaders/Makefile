CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g -Wall -I/usr/include/SDL2
LDFLAGS=-g
LDLIBS=-lSDL2 -lSDL2_image -lSDL2_gfx -lSDL2_ttf

SPRITE_SRCS=../sdl_sprite/Sprite.cpp 
SPRITE_OBJS=$(subst .cpp,.o,$(SPRITE_SRCS))

NAME=invaders
TEST_SRCS=$(NAME).cpp
TEST_OBJS=$(subst .cpp,.o,$(TEST_SRCS)) 

all: $(NAME)

$(NAME): $(TEST_OBJS) $(SPRITE_OBJS)
	$(CXX) $(LDFLAGS) -o $(NAME) $(TEST_OBJS) $(SPRITE_OBJS) $(LDLIBS)

sdl_sprite_test.o: sdl_sprite_test.cpp 

Sprite.o: ../sdl_sprite/Sprite.h ../sdl_sprite/Sprite.cpp

clean:
	$(RM) $(SPRITE_OBJS) $(TEST_OBJS) $(NAME)

distclean: clean
	$(RM) $(NAME)
