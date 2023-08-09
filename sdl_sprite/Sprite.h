#ifndef Sprite_h
#define Sprite_h

#include <cstdlib>

#include <iostream>
#include <string>

#include <SDL.h>
#include <SDL_image.h>
//#include <SDL2_gfxPrimitives.h>

typedef enum {
	SPRITE_TYPE_NONE,
    SPRITE_TYPE_IMAGE,
    SPRITE_TYPE_SHAPE,
    SPRITE_TYPE_SSHEET
} SPRITE_TYPE;
typedef enum {
	SPRITE_SHAPE_NONE,
    SPRITE_SHAPE_RECT,
    SPRITE_SHAPE_CIRCLE
} SPRITE_SHAPE;

#define MAX_ANIM_FRAMES 10

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
    // SPRITE_TYPE_SSHEET
    Sprite(SDL_Rect *rect);
    Sprite(SDL_Rect *rect, int fps);
    
    bool draw();
    bool update();

    void setWrap(bool val);
    void setPos(int x, int y);
    void setVel(float velx, float vely);
    void setGravity(bool b);
    void setLifetime(Uint32 ticks, bool fade);

    void setBorderColor(int r, int g, int b);
    void setBorderColor(int r, int g, int b, int a);

    void setSpriteDestSize(int w, int h);
    void setSpriteZoom(float zoom);

    bool addAnimImage(std::string path);
    bool addAnimSprite(SDL_Rect *ssrect);
    void setAnimTime(Uint32 ms);
    void setFrameTime(Uint32 ms);
    void delayStartTime(Uint32 ms);
    void setMoveSmooth(int sx, int sy);

    static bool loadSheet( std::string path );
    static void setRenderer(SDL_Renderer *_renderer);
    static SDL_Texture *getSheet();

    int getX();
    int getY();
    float getVX();
    float getVY();
    int getW();
    int getH();
    SDL_Rect *getPos();

    void incX(float incx);
    void incY(float invy);
    void invVX(float incvx);
    void incVY(float invvy);

    void revVY();
    void revVX();

    SPRITE_TYPE type;
    bool loaded;
    bool dead;
    bool run;
    bool wrap;

    float fps;

private:
    static SDL_Renderer *renderer;
    static int win_width;
    static int win_height;

    SDL_Rect pos;  // position and size of sprite (on screen)
    float vx;
    float vy;

    float x;
    float y;
    Uint32 last_update_tick;
    Uint32 frame_update_time_ms;
    Uint32 created_tick;

    // For type = Image
    SDL_Texture* sprite[MAX_ANIM_FRAMES];

    // For type = Shape
    SPRITE_SHAPE shape;
    SDL_Color col;    // fill color
    SDL_Color bcol;   // border

    // For Animation
    int num_frames;
    int current_frame;
    Uint32 anim_update_time_ms;
    Uint32 last_anim_update_tick;

    // For type Sprite Sheet
    static SDL_Texture *sheet;
    SDL_Rect sspos[MAX_ANIM_FRAMES];
    static int sheet_width;
    static int sheet_height;
    static bool sheet_loaded;
    
    // simulate gravity down
    bool gravity;
    Uint32 lifetime; // lifetime in ticks. If == 0 then it does not die.
    bool lifetimeFade;
    float fadeValue;

    int smoothToX;
    int smoothToY;

    SDL_Texture* loadTexture( std::string path );
    
};

#endif
