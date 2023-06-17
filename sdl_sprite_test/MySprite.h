#ifndef MySprite_h
#define MySprite_h

#include <cstdlib>

#include <iostream>
#include <string>

#include <SDL.h>
#include <SDL_image.h>

typedef enum {
    MYSPRITE_TYPE_IMAGE,
    MYSPRITE_TYPE_RECT
} MYSPRITE_TYPE;

class MySprite {
public: 
    MySprite();
    virtual ~MySprite();
    MySprite* destroy();
    // MYSPRITE_TYPE_IMAGE
    MySprite(SDL_Renderer *_r, std::string path);
    MySprite(SDL_Renderer *_r, std::string path, int fps);
    // MYSPRITE_TYPE_RECT
    MySprite(SDL_Renderer *_r, int w, int h, SDL_Color *col);
    MySprite(SDL_Renderer *_r, int w, int h, SDL_Color *col, int fps);
    MySprite(SDL_Renderer *_r, int w, int h, int r, int g, int b, int a, int fps);
    bool draw();
    void setWrap(bool val);
    void setPos(int x, int y);
    void setVel(float velx, float vely);
    bool update();

    void setBorderColor(int r, int g, int b);
    void setBorderColor(int r, int g, int b, int a);

    void setLifetime(Uint32 ticks);

private:
    SDL_Renderer *renderer;
    MYSPRITE_TYPE type;
    bool loaded;
    bool dead;
    bool run;
    bool wrap;
    int win_width;
    int win_height;
    float vx;
    float vy;
    float x;
    float y;
    SDL_Rect pos;
    Uint32 last_update_tick;
    Uint32 frame_update_time_ms;
    Uint32 created_tick;
    int sprite_width;
    int sprite_height;

    // For type = Image
    SDL_Texture* sprite;

    // For type = Rect
    SDL_Color col;    // fill color
    SDL_Color bcol;   // border

    // For type = Animation
    int maxFrames;

    // lifetime in ticks. If == 0 then it does not die.
    Uint32 lifetime;

    SDL_Texture* loadTexture( std::string path );
};

#endif