CC=gcc
CXX=g++
RM=rm -f
CPPFLAGS=-g -Wall 
#-Wl,-subsystem,windows
CPPFLAGS+=-I C:/libraries/SDL2/x86_64-w64-mingw32/include/SDL2 
LDFLAGS=-L C:/libraries/SDL2/x86_64-w64-mingw32/lib/
CPPFLAGS+=-I C:/libraries/SDL2_image/x86_64-w64-mingw32/include/SDL2 
LDFLAGS+=-L C:/libraries/SDL2_image/x86_64-w64-mingw32/lib/
CPPFLAGS+=-I C:/libraries/SDL2_mixer-2.6.3/x86_64-w64-mingw32/include/SDL2 
LDFLAGS+=-L C:/libraries/SDL2_mixer-2.6.3/x86_64-w64-mingw32/lib/

LDLIBS=-lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_mixer

STATICLIBS=-lmingw32 -lSDL2main 
STATICLIBS+=C:/libraries/SDL2/x86_64-w64-mingw32/lib/libSDL2.a 
STATICLIBS+=C:/libraries/SDL2_image/x86_64-w64-mingw32/lib/libSDL2_image.a
STATICLIBS+=C:/libraries/SDL2_mixer-2.6.3/x86_64-w64-mingw32/lib/libSDL2_mixer.a
STATICLIBS+=-mwindows -Wl,--dynamicbase -Wl,--nxcompat -Wl,--high-entropy-va -lm -ldinput8 -ldxguid -ldxerr8 -luser32 -lgdi32 -lwinmm -limm32 -lole32 -loleaut32 -lshell32 -lsetupapi -lversion -luuid

SPRITE_SRCS=../sdl_sprite/Sprite.cpp 
SPRITE_OBJS=$(subst .cpp,.o,$(SPRITE_SRCS))

INVADERS_EXEC=../invaders/invaders.exe
INVADERS_SRCS=$(subst .exe,.cpp, $(INVADERS_EXEC))
INVADERS_OBJS=$(subst .cpp,.o,$(INVADERS_SRCS)) 

SDLRENDERER_EXEC=../sdl_renderer_hello/hello.exe
SDLRENDERER_SRCS=$(subst .exe,.cpp,$(SDLRENDERER_EXEC)) 
SDLRENDERER_OBJS=$(subst .cpp,.o,$(SDLRENDERER_SRCS)) 

SDLHELLO_EXEC=../sdl_hello/hello.exe
SDLHELLO_SRCS=$(subst .exe,.cpp,$(SDLHELLO_EXEC)) 
SDLHELLO_OBJS=$(subst .cpp,.o,$(SDLHELLO_SRCS)) 

SDLTEST_EXEC=../sdl_renderer_test/hello.exe
SDLTEST_SRCS=$(subst .exe,.cpp,$(SDLTEST_EXEC)) 
SDLTEST_OBJS=$(subst .cpp,.o,$(SDLTEST_SRCS)) 

all: invaders sdl_renderer_hello sdl_hello sd_renderer_test

.PHONY: invaders
invaders: $(INVADERS_EXEC)

sdl_renderer_hello: $(SDLRENDERER_EXEC)

sdl_hello: $(SDLHELLO_EXEC)

sdl_renderer_test: $(SDLTEST_EXEC)

$(INVADERS_EXEC): $(INVADERS_SRCS) ../sdl_sprite/Sprite.h ../sdl_sprite/Sprite.cpp
	@echo "Make $(INVADERS_EXEC)"
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o $(INVADERS_EXEC) $(INVADERS_SRCS) ../sdl_sprite/Sprite.cpp $(LDLIBS)

$(SDLRENDERER_EXEC): $(SDLRENDERER_SRCS)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o $(SDLRENDERER_EXEC) $(SDLRENDERER_SRCS) $(LDLIBS) $(STATICLIBS)

$(SDLHELLO_EXEC): $(SDLHELLO_SRCS)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o $(SDLHELLO_EXEC) $(SDLHELLO_SRCS) $(LDLIBS) $(STATICLIBS)

$(SDLTEST_EXEC): $(SDLTEST_SRCS)
	$(CXX) $(CPPFLAGS) $(LDFLAGS) -o $(SDLTEST_EXEC) $(SDLTEST_SRCS) $(LDLIBS)

clean:
	$(RM) $(SPRITE_OBJS) $(INVADERS_OBJS) $(SDLRENDERER_OBJS) $(SDLHELLO_OBJS) $(SDLTEST_OBJS)

distclean: clean
	$(RM) $(INVADERS_EXEC) $(SDLRENDERER_EXEC) $(SDLHELLO_EXEC) $(SDLTEST_EXEC)
