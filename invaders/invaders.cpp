#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "../sdl_sprite/Sprite.h"

#define INV_FPS 5

//Screen dimension constants
const int SCREEN_WIDTH  = 448;
const int SCREEN_HEIGHT = 512;

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

// Imge to create a sprite from
std::string gSpriteSheetPath = "../resources/textures/sprite_sheet_space_invaders.jpg";

//Rendered texture
SDL_Texture* gTextTexture;
int gTextTextureWidth, gTextTextureHeight;

Sprite *inv[55];

void addParticle(int px, int py, int vx, int vy);
void updateAndDrawParticles();

bool myinit() 
{

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL_Error: %s\n", SDL_GetError() );
        return false;
    }
    else
    {
   		//Set texture filtering to linear
		if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
		{
			printf( "Warning: Linear texture filtering not enabled!" );
		}

        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL_Error: %s\n", SDL_GetError() );
            return false;
        }
        else
        {
			//Create renderer for window
			gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED );
			if( gRenderer == NULL )
			{
				printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
				return false;
			}
			else
			{
                // SDL_RendererInfo info;
                // SDL_GetRendererInfo(gRenderer, &info);
                // printf("Renderer\n\tFlags 0x%X\n\tnum formats %d\n",info.flags, info.num_texture_formats);
                // for (int n = 0; n < info.num_texture_formats; n++) {
                //     printf("\t\t0x%X\n",info.texture_formats[n]);
                // }
                // printf("\n");
				//Initialize renderer color
				SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );

				//Initialize PNG loading
				int imgFlags = IMG_INIT_PNG;
				if( !( IMG_Init( imgFlags ) & imgFlags ) )
				{
					printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
					return false;
				}
			}
        }
    }

    return true;
}

int invVel = 2;

int main( int argc, char* args[] )
{
   //Start up SDL and create window
    if( !myinit() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        SDL_Event e; 
        bool quit = false; 

        Sprite::setRenderer(gRenderer);

        // Load a Sprite Sheet and create a sprite from it
        Sprite::loadSheet(gSpriteSheetPath);
        // {  7, 225, 16, 16} Invader1 pos1
        // { 40, 225, 16, 16} Invader1 pos2
        // { 74, 225, 22, 16} Invader2 pos1
        // {107, 225, 22, 16} Invader2 pos2
        // {147, 226, 24, 16} Invader3 pos1
        // {179, 226, 24, 16} Invader4 pos2

        SDL_Rect sspos1, sspos2;
        sspos1 = { 7, 225, 16, 16};
        sspos2 = {40, 225, 16, 16};
        for (int i=0;i<11;i++)
        {
            inv[i] = new Sprite(&sspos1,INV_FPS);
            inv[i]->addAnimSprite(&sspos2);
            inv[i]->setPos(48+32*i+4,128);
            inv[i]->setVel(0,0);
            inv[i]->setWrap(true);
            inv[i]->setSpriteZoom(1); 
        }
        sspos1 = { 74, 225, 22, 16};
        sspos2 = {107, 225, 22, 16};
        for (int i=0;i<11;i++)
        {
            inv[i+11] = new Sprite(&sspos1, INV_FPS);
            inv[i+11]->addAnimSprite(&sspos2);
            inv[i+11]->setPos(48+32*i+2,128+32);
            inv[i+11]->setVel(0,0);
            inv[i+11]->setWrap(true);
            inv[i+11]->setSpriteZoom(1); 
            inv[i+22] = new Sprite(&sspos1, INV_FPS);
            inv[i+22]->addAnimSprite(&sspos2);
            inv[i+22]->setPos(48+32*i+2,128+64);
            inv[i+22]->setVel(0,0);
            inv[i+22]->setWrap(true);
            inv[i+22]->setSpriteZoom(1); 
        }
        sspos1 = {147, 225, 24, 16};
        sspos2 = {179, 225, 24, 16};
        for (int i=0;i<11;i++)
        {
            inv[i+33] = new Sprite(&sspos1, INV_FPS);
            inv[i+33]->addAnimSprite(&sspos2);
            inv[i+33]->setPos(48+32*i+2,128+96);
            inv[i+33]->setVel(0,0);
            inv[i+33]->setWrap(true);
            inv[i+33]->setSpriteZoom(1); 
            inv[i+44] = new Sprite(&sspos1, INV_FPS);
            inv[i+44]->addAnimSprite(&sspos2);
            inv[i+44]->setPos(48+32*i+2,128+128);
            inv[i+44]->setVel(0,0);
            inv[i+44]->setWrap(true);
            inv[i+44]->setSpriteZoom(1); 
        }

        while( quit == false )
        { 
            while( SDL_PollEvent( &e ) )
            { 
                if( e.type == SDL_QUIT )
                {
                    quit = true; 
                } 
                else if (e.type == SDL_KEYDOWN) 
                {
                    //printf("Key 0x%X\n",e.key.keysym.sym);
                    switch (e.key.keysym.sym)
                    {
                        case SDLK_UP:
                        break;
                        case SDLK_DOWN:
                        break;
                        case SDLK_LEFT:
                        break;
                        case SDLK_RIGHT:
                        break;
                        case SDLK_q:
                        quit = true;
                        break;
                        default:
                        break;
                    }
                }

            } 

            //Initialize renderer color
            SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );

            //Clear screen
            SDL_RenderClear( gRenderer );

            for (int i=0;i<55;i++) {
                if (inv[i]) {
                    inv[i]->update();
                    inv[i]->draw();
                }
            }

            //Update screen
            SDL_RenderPresent( gRenderer );
        }
    }

    // CLOSE

    // delete sprites
    for (int i=0;i<55;i++) {
        inv[i]->destroy();
    }

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    SDL_QuitSubSystem(SDL_INIT_VIDEO);
    SDL_Quit();
    return 0;
}

