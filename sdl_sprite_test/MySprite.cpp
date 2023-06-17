#include "MySprite.h"

MySprite::MySprite() {
    sprite = NULL;
    loaded = false;
    dead = false;
    wrap = false;
    sprite_width = 0;
    sprite_height = 0;
    win_width = 0;
    win_height = 0;
    run = false;
    pos = {0,0,0,0};
    vx = 0.0; vy = 0.0;
    x = 0.0; y = 0.0;
    lifetime = 0; // stay alive
}

MySprite::~MySprite() {
    destroy();
}
MySprite* MySprite::destroy() {
    if (type == MYSPRITE_TYPE_IMAGE && sprite) {
        SDL_DestroyTexture(sprite);
    }
    return this;
}

MySprite::MySprite(SDL_Renderer *_r, std::string path) : MySprite(_r, path, 30) {}
MySprite::MySprite(SDL_Renderer *_r, std::string path, int fps) {
    //printf("Create Image Sprite from single image\n");
    this->renderer = _r;
    SDL_GetRendererOutputSize(this->renderer, &(this->win_width), &(this->win_height));
    
    this->frame_update_time_ms = (int)(1000.0 / (float)fps);
    this->last_update_tick = this->created_tick = SDL_GetTicks();
    

    this->type = MYSPRITE_TYPE_IMAGE;
    sprite = loadTexture(path);
    if (this->sprite) this->loaded = true;  
    this->dead = false;  
}

MySprite::MySprite(SDL_Renderer *_r, int w, int h, SDL_Color *col, int fps) {
    //printf("Create Rect Sprite\n");
    this->renderer = _r;
    SDL_GetRendererOutputSize(this->renderer, &(this->win_width), &(this->win_height));
    
    this->frame_update_time_ms = (int)(1000.0 / (float)fps);
    this->last_update_tick = this->created_tick = SDL_GetTicks();

    this->type = MYSPRITE_TYPE_RECT;
    this->sprite_width = w;
    this->sprite_height = h;
    this->pos.w = w;
    this->pos.h = h;
    this->col.r = this->bcol.r = col->r;
    this->col.g = this->bcol.g = col->g;
    this->col.b = this->bcol.b = col->b;
    this->col.a = this->bcol.a = col->a;

    this->dead = false;  
}

MySprite::MySprite(SDL_Renderer *_r, int w, int h, int r, int g, int b, int a, int fps) {
    //printf("Create Rect Sprite\n");
    this->renderer = _r;
    SDL_GetRendererOutputSize(this->renderer, &(this->win_width), &(this->win_height));
    
    this->frame_update_time_ms = (int)(1000.0 / (float)fps);
    this->last_update_tick = this->created_tick = SDL_GetTicks();

    this->type = MYSPRITE_TYPE_RECT;
    this->sprite_width = w;
    this->sprite_height = h;
    this->pos.w = w;
    this->pos.h = h;
    this->col.r = this->bcol.r = r;
    this->col.g = this->bcol.g = g;
    this->col.b = this->bcol.b = b;
    this->col.a = this->bcol.a = a;

    this->dead = false;  

}

//Loads individual image as texture
SDL_Texture* MySprite::loadTexture( std::string path )
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

        if (SDL_QueryTexture(newTexture,NULL, NULL, &sprite_width, &sprite_height)!=0) {
            printf("Unable to get Texture Size info\n");
        }
        printf("Sprite loaded %dx%d\n", sprite_width, sprite_height);
        pos.w = sprite_width;
        pos.h = sprite_height;
	}

	return newTexture;
}

bool MySprite::draw() {
    if (dead) return false;
    if (type==MYSPRITE_TYPE_IMAGE) 
    {
        if (!loaded) {
            printf("No sprite?\n");
            return false;
        }
        //Render texture to screen
        SDL_RenderCopy( renderer, sprite, NULL, &pos );
        return true;
    }
    if (type==MYSPRITE_TYPE_RECT) 
    {
        SDL_SetRenderDrawColor(renderer, col.r, col.g, col.b, col.a );
        SDL_RenderFillRect(renderer, &pos);
        SDL_SetRenderDrawColor(renderer, bcol.r, bcol.g, bcol.b, bcol.a );
        SDL_RenderDrawRect(renderer, &pos);
        return true;
    } 
    
    return false;
}

void MySprite::setWrap(bool val) {
    wrap = val;
}

void MySprite::setPos(int _x, int _y) {
    pos.x = _x;
    pos.y = _y;
    x = (float) _x;
    y = (float) _y;
}

void MySprite::setVel(float velx, float vely) {
    vx = velx;
    vy = vely;
}

bool MySprite::update() {
    if (dead) return false;
    if (lifetime != 0 && SDL_TICKS_PASSED(SDL_GetTicks(), created_tick + lifetime)) {
        dead = true;
        return false;
    }
    // update at set frame-rate (or slower)
    if (!SDL_TICKS_PASSED(SDL_GetTicks(),last_update_tick + frame_update_time_ms)) {
        return true;
    }
    last_update_tick = SDL_GetTicks();


    x += vx; 
    y += vy;
    if (wrap) 
    {
        if (x > win_width) {x -= (float)(win_width + sprite_width); };
        if (x < 0-sprite_width) {x += (float)(win_width + sprite_width); };
        
        if (y > win_height) {y -= (float)(win_height + sprite_height); };
        if (y < 0-sprite_height) {x += (float)(win_height + sprite_height); };
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

void MySprite::setBorderColor(int r, int g, int b) {
    this->setBorderColor(r, g, b, 255);
}

void MySprite::setBorderColor(int r, int g, int b, int a)
{
    bcol.r = r;
    bcol.g = g;
    bcol.b = b;
    bcol.a = a;
}

void MySprite::setLifetime(Uint32 ticks) {
    lifetime = ticks; // measured in ms
}