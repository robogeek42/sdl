#ifndef MySprite_h
#define MySprite_h

#include <cstdlib>

#include <iostream>
#include <string>

#include <SDL.h>
#include <SDL_image.h>

class MySprite {
public: 
    MySprite();
    virtual ~MySprite();
    MySprite* destroy();
    MySprite(SDL_Renderer *r, std::string path);
    bool draw();
    void setPos(int x, int y);

private:
    SDL_Renderer *renderer;
    bool loaded;
    bool run;
    int speed;
    int width;
    int height;
    int maxFrames;
    SDL_Rect pos;

    SDL_Texture* sprite;

    SDL_Texture* loadTexture( std::string path );
};

#endif