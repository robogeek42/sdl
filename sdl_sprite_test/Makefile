CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g -Wall -I/usr/include/SDL2
LDFLAGS=-g
LDLIBS=-lSDL2 -lSDL2_image

SRCS=Sprite.cpp sdl_sprite_test.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: sdl_sprite_test

sdl_sprite_test: $(OBJS)
	$(CXX) $(LDFLAGS) -o sdl_sprite_test $(OBJS) $(LDLIBS)

sdl_sprite_test.o: sdl_sprite_test.cpp Sprite.cpp

Sprite.o: Sprite.h Sprite.cpp

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) sdl_sprite_test