CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g -Wall -I/usr/include/SDL2
LDFLAGS=-g
LDLIBS=-lSDL2 -lSDL2_image

SRCS=MySprite.cpp sdl_sprite_test.cpp
OBJS=$(subst .cpp,.o,$(SRCS))

all: sdl_sprite_test

sdl_sprite_test: $(OBJS)
	$(CXX) $(LDFLAGS) -o sdl_sprite_test $(OBJS) $(LDLIBS)

sdl_sprite_test.o: sdl_sprite_test.cpp MySprite.cpp

MySprite.o: MySprite.h MySprite.cpp

clean:
	$(RM) $(OBJS)

distclean: clean
	$(RM) sdl_sprite_test
