#ifndef Sprite_h
#define Sprite_h

#include <cstdlib>

#include <iostream>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
#include <SDL2_gfxPrimitives.h>

typedef enum {
    SPRITE_TYPE_IMAGE,
    SPRITE_TYPE_SHAPE
} SPRITE_TYPE;
typedef enum {
    SPRITE_SHAPE_RECT,
    SPRITE_SHAPE_CIRCLE
} SPRITE_SHAPE;

class Sprite {
public: 
    Sprite();
    virtual ~Sprite();
    Sprite* destroy();
    // SPRITE_TYPE_IMAGE
    Sprite(SDL_Renderer *_r, std::string path);
    Sprite(SDL_Renderer *_r, std::string path, int fps);
    // SPRITE_TYPE_SHAPE
    Sprite(SDL_Renderer *_r, SPRITE_SHAPE _shape, int w, int h, SDL_Color *col);
    Sprite(SDL_Renderer *_r, SPRITE_SHAPE _shape, int w, int h, SDL_Color *col, int fps);
    Sprite(SDL_Renderer *_r, SPRITE_SHAPE _shape, int w, int h, int r, int g, int b, int a, int fps);
    bool draw();
    void setWrap(bool val);
    void setPos(int x, int y);
    void setVel(float velx, float vely);
    bool update();
    void setGravity(bool b);

    void setBorderColor(int r, int g, int b);
    void setBorderColor(int r, int g, int b, int a);

    void setLifetime(Uint32 ticks, bool fade);

private:
    SDL_Renderer *renderer;
    SPRITE_TYPE type;
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

    // For type = Shape
    SPRITE_SHAPE shape;
    SDL_Color col;    // fill color
    SDL_Color bcol;   // border

    // For type = Animation
    int maxFrames;

    // simulate gravity down
    bool gravity;
    // lifetime in ticks. If == 0 then it does not die.
    Uint32 lifetime;
    bool lifetimeFade;
    float fadeValue;

    SDL_Texture* loadTexture( std::string path );
};

#endif
