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
    Sprite(std::string path);
    Sprite(std::string path, int fps);
    // SPRITE_TYPE_SHAPE
    Sprite(SPRITE_SHAPE _shape, int w, int h, SDL_Color *col);
    Sprite(SPRITE_SHAPE _shape, int w, int h, SDL_Color *col, int fps);
    Sprite(SPRITE_SHAPE _shape, int w, int h, int r, int g, int b, int a, int fps);
    bool draw();
    void setWrap(bool val);
    void setPos(int x, int y);
    void setVel(float velx, float vely);
    bool update();
    void setGravity(bool b);

    void setBorderColor(int r, int g, int b);
    void setBorderColor(int r, int g, int b, int a);

    void setLifetime(Uint32 ticks, bool fade);
    static bool loadSheet( std::string path );
    static void setRenderer(SDL_Renderer *_renderer);

private:
    static SDL_Renderer *renderer;
    static int win_width;
    static int win_height;

    SPRITE_TYPE type;
    bool loaded;
    bool dead;
    bool run;
    bool wrap;
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

    static SDL_Texture *sheet;
    static int sheet_width;
    static int sheet_height;
    static bool sheet_loaded;
    #define MAX_SHEET_SPRITES 100
    static SDL_Rect ssprites[MAX_SHEET_SPRITES];
    static int num_ssprites;

    SDL_Texture* loadTexture( std::string path );
    
};

#endif
