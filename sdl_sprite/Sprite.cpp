#include "Sprite.h"

// initialise static members
SDL_Renderer* Sprite::renderer = NULL;
int Sprite::win_width = 0;
int Sprite::win_height = 0;
SDL_Texture* Sprite::sheet = NULL;
bool Sprite::sheet_loaded = false;
int Sprite::sheet_width = 0;
int Sprite::sheet_height = 0;

Sprite::Sprite() {
    for (int i=0; i<MAX_ANIM_FRAMES; i++) {
        sprite[i] = NULL;
    }
    loaded = false;
    wrap = false;
    run = false;
    pos = {0,0,0,0};
    vx = 0.0; vy = 0.0;
    x = 0.0; y = 0.0;
    dead = false;
    gravity = false;
    lifetime = 0; // stay alive
    lifetimeFade = false;
    fadeValue = 0.0;
    num_frames = 0;  // animation is off if < 2
    current_frame = 0;
}

Sprite::~Sprite() {
    destroy();
}
Sprite* Sprite::destroy() {
    if (type == SPRITE_TYPE_IMAGE && loaded) {
        for (int i=0; i<MAX_ANIM_FRAMES; i++) {
            if (sprite[i]) {
                SDL_DestroyTexture(sprite[i]);
            }
        }
    }
    if (type == SPRITE_TYPE_SSHEET && sheet_loaded) {
        SDL_DestroyTexture(sheet);
    }
    return this;
}

void Sprite::setRenderer(SDL_Renderer *_renderer) {
    renderer = _renderer;
    SDL_GetRendererOutputSize(renderer, &win_width, &win_height);
}

// Image type
Sprite::Sprite(std::string path) : Sprite(path, 30) {}
Sprite::Sprite(std::string path, int fps) {
    //printf("Create Image Sprite from single image\n");
    
    this->frame_update_time_ms = (int)(1000.0 / (float)fps);
    this->last_update_tick = this->created_tick = SDL_GetTicks();
    this->last_anim_update_tick = this->last_update_tick;
    this->anim_update_time_ms = 500;

    this->type = SPRITE_TYPE_IMAGE;
    this->sprite[0] = loadTexture(path);
    this->current_frame = 0;
    if (this->sprite) {
        this->loaded = true;  
        this->num_frames = 1;
        this->dead = false;  
        this->gravity = false;
        this->lifetime = 0;
        this->lifetimeFade = false;
        this->fadeValue = 0.0;
    }
}

// Shape type
Sprite::Sprite(SPRITE_SHAPE _shape, int w, int h, SDL_Color *col, int fps) 
: Sprite(_shape, w, h, col->r, col->g, col->b, col->a, fps) {}

Sprite::Sprite(SPRITE_SHAPE _shape, int w, int h, int r, int g, int b, int a, int fps) {
    //printf("Create Rect Sprite\n");
    this->frame_update_time_ms = (int)(1000.0 / (float)fps);
    this->last_update_tick = this->created_tick = SDL_GetTicks();
    this->last_anim_update_tick = this->last_update_tick;
    this->anim_update_time_ms = 500;

    this->type = SPRITE_TYPE_SHAPE;
    this->shape = _shape;
    this->pos.w = w;
    this->pos.h = h;
    this->col.r = this->bcol.r = r;
    this->col.g = this->bcol.g = g;
    this->col.b = this->bcol.b = b;
    this->col.a = this->bcol.a = a;

    this->dead = false;  
    this->gravity = false;
    this->lifetime = 0;
    this->lifetimeFade = false;
    this->fadeValue = 1.0;
    this->current_frame = 0;
    // if (!SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND)) {
    //     printf("Failed to set renderer blend mode\n");
    // }

}

// Sprite-sheet type
Sprite::Sprite(SDL_Rect *rect) : Sprite(rect, 30) {}
Sprite::Sprite(SDL_Rect *rect, int fps) {
    this->frame_update_time_ms = (int)(1000.0 / (float)fps);
    this->last_update_tick = this->created_tick = SDL_GetTicks();
    this->last_anim_update_tick = this->last_update_tick;
    this->anim_update_time_ms = 500;

    this->type = SPRITE_TYPE_SSHEET;
    this->current_frame = 0;
    if (sheet_loaded) {
        this->sspos[0] = *rect;
        this->num_frames = 1;
        this->pos.w = rect->w;
        this->pos.h = rect->h;
        this->loaded = true;  
        this->dead = false;  
        this->gravity = false;
        this->lifetime = 0;
        this->lifetimeFade = false;
        this->fadeValue = 0.0;
    }
}

//Loads individual image as texture
SDL_Texture* Sprite::loadTexture( std::string path )
{
	//The final texture
	SDL_Texture* newTexture = NULL;

	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( renderer, loadedSurface );
		if( newTexture == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );

        if (SDL_QueryTexture(newTexture,NULL, NULL, &pos.w, &pos.h)!=0) {
            printf("Unable to get Texture Size info\n");
        }
        printf("Sprite loaded %dx%d\n", pos.w, pos.h);
	}

	return newTexture;
}

bool Sprite::draw() {
    if (dead) return false;
    if (type==SPRITE_TYPE_IMAGE || type==SPRITE_TYPE_SSHEET) 
    {
        if (!loaded) {
            printf("No sprite?\n");
            return false;
        }
        //Render texture to screen
        if (type==SPRITE_TYPE_IMAGE) 
        {
            SDL_RenderCopy( renderer, sprite[current_frame], NULL, &pos );
        }
        if (type==SPRITE_TYPE_SSHEET) 
        {
            SDL_RenderCopy( renderer, sheet, &sspos[current_frame], &pos );
        }
        return true;
    }
    if (type==SPRITE_TYPE_SHAPE) 
    {
        switch (shape) {
            case SPRITE_SHAPE_RECT: 
            {
                SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a *fadeValue);
                SDL_RenderFillRect(renderer, &pos);
                SDL_SetRenderDrawColor(renderer, bcol.r, bcol.g, bcol.b, bcol.a *fadeValue );
                SDL_RenderDrawRect(renderer, &pos);
                break;
            }
            case SPRITE_SHAPE_CIRCLE:
            {
                filledCircleRGBA(renderer, pos.x, pos.y, pos.w/2, col.r, col.g, col.b, col.a * fadeValue);
                break;
            }
            default:
                printf("Shape Type unknown\n");
            break;
        }
        return true;
    } 

    return false;
}

void Sprite::setWrap(bool val) {
    wrap = val;
}

void Sprite::setPos(int _x, int _y) {
    pos.x = _x;
    pos.y = _y;
    x = (float) _x;
    y = (float) _y;
}

void Sprite::setVel(float velx, float vely) {
    vx = velx;
    vy = vely;
}

int Sprite::getX() {
	return pos.x;
}
int Sprite::getY() {
	return pos.y;
}
float Sprite::getVX() {
	return vx;
}
float Sprite::getVY() {
	return vy;
}

void Sprite::incX(float incx) {
	x += incx;
	pos.x  = (int) x;
}
void Sprite::incY(float incy) {
	y += incy;
	pos.y  = (int) y;
}
void Sprite::invVX(float incvx) {
	vx += incvx;
}
void Sprite::incVY(float incvy) {
	vy += incvy;
}

void Sprite::revVX() {
	vx *= -1;
}
void Sprite::revVY() {
	vy *= -1;
}

void Sprite::setGravity(bool b) {
    gravity = b;
    return;
}

bool Sprite::update() {
    if (dead) return false;

    // kill this sprite after lifetime ms
    if (lifetime != 0 && SDL_TICKS_PASSED(SDL_GetTicks(), created_tick + lifetime)) {
        dead = true;
        return false;
    }
    // update animation
    if (num_frames > 1 && SDL_TICKS_PASSED(SDL_GetTicks(), last_anim_update_tick + anim_update_time_ms)) {
        current_frame++;
        current_frame %= num_frames;
        last_anim_update_tick = SDL_GetTicks();
    }
    // update at set frame-rate (or slower)
    if (!SDL_TICKS_PASSED(SDL_GetTicks(),last_update_tick + frame_update_time_ms)) {
        return true;
    }
    last_update_tick = SDL_GetTicks();

    // fade when 1 second remains
    if (lifetimeFade && lifetime != 0)
    { 
        int ticks_fading_starts = created_tick + std::max((int)0, (int)(lifetime - 1000));
        if (SDL_TICKS_PASSED(SDL_GetTicks(), ticks_fading_starts)) {
            int fade = 255 - (SDL_GetTicks() - ticks_fading_starts)/4;   
            fadeValue = (float)fade / 255.0;
        }
    }

    // gravity
    if (gravity == true) {
        vy += (4.0 * frame_update_time_ms)/1000.0;
    }

    x += vx; 
    y += vy;
    if (wrap) 
    {
        if (x > win_width) {x -= (float)(win_width + pos.w); };
        if (x < 0-pos.w) {x += (float)(win_width + pos.w); };
        
        if (y > win_height) {y -= (float)(win_height + pos.h); };
        if (y < 0-pos.h) {x += (float)(win_height + pos.h); };
    } else {
        if (x > win_width*2) {dead = true; }
        if (x < 0-win_width) {dead = true; }
        
        if (y > win_height*2) {dead = true; }
        if (y < 0-win_height) {dead = true; }
    }

    pos.x = (int) x;
    pos.y = (int) y;
    return true;
}

void Sprite::setBorderColor(int r, int g, int b) {
    this->setBorderColor(r, g, b, 255);
}

void Sprite::setBorderColor(int r, int g, int b, int a)
{
    bcol.r = r;
    bcol.g = g;
    bcol.b = b;
    bcol.a = a;
}

void Sprite::setSpriteDestSize(int w, int h) {
    pos.w = w;
    pos.h = h;
}

void Sprite::setSpriteZoom(float zoom) {
    pos.w = int(zoom * pos.w);
    pos.h = int(zoom * pos.h);
}


void Sprite::setLifetime(Uint32 ticks, bool fade) {
    lifetime = ticks; // measured in ms
    lifetimeFade = fade;
}

bool Sprite::loadSheet( std::string path )
{
    bool retval = true;
   	//Load image at specified path
	SDL_Surface* loadedSurface = IMG_Load( path.c_str() );
	if( loadedSurface == NULL )
	{
		printf( "Unable to load image %s! SDL_image Error: %s\n", path.c_str(), IMG_GetError() );
	}
	else
	{
		//Create texture from surface pixels
        sheet = SDL_CreateTextureFromSurface( renderer, loadedSurface );
		if( sheet == NULL )
		{
			printf( "Unable to create texture from %s! SDL Error: %s\n", path.c_str(), SDL_GetError() );
            retval = false;
		}

		//Get rid of old loaded surface
		SDL_FreeSurface( loadedSurface );

        if (SDL_QueryTexture(sheet, NULL, NULL, &(sheet_width), &(sheet_height))!=0) {
            printf("Unable to get Texture Size info\n");
            retval = false;
        } else {
            printf("Sprite sheet loaded %dx%d\n", sheet_width, sheet_height);
        }
	}
    sheet_loaded = retval;
	return retval;
}

bool Sprite::addAnimImage(std::string path)
{
    bool retval = true;
    int index = num_frames;

    if (num_frames < MAX_ANIM_FRAMES && type == SPRITE_TYPE_IMAGE && loaded) {
        sprite[index] = loadTexture(path);
        if (sprite[index] == NULL) {
            retval = false;
        } else {
            num_frames++;
        }
    }
    return retval;
}
bool Sprite::addAnimSprite(SDL_Rect *ssrect)
{
    bool retval = true;
    int index = num_frames;

    if (num_frames < MAX_ANIM_FRAMES && type == SPRITE_TYPE_SSHEET && sheet_loaded) {
        sspos[index] = *ssrect;
        num_frames++;

    } else {
        retval = false;
    }

    return retval;
}

void Sprite::setAnimTime(Uint32 ms) 
{
    // default is 500ms
    anim_update_time_ms = ms;
}

void Sprite::setFrameTime(Uint32 ms) 
{
    // Was set as the FPS time
    frame_update_time_ms = ms;
}
