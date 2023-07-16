#include <SDL.h>
#include <SDL_image.h>
#include <stdio.h>
#include <string>
#include "../sdl_sprite/Sprite.h"

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 720;

//Starts up SDL and creates window
bool init();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

// Imge to create a sprite from
std::string gImagePath = "../resources/textures/UFO_1_Enlarged.png";
std::string gImagePath2 = "../resources/textures/UFO_2_Enlarged.png";
std::string gSpriteSheetPath = "../resources/textures/sprite_sheet_space_invaders.jpg";

//Rendered texture
SDL_Texture* gTextTexture;
int gTextTextureWidth, gTextTextureHeight;

Sprite *s1, *s2;
Sprite *inv[10];
#define NUM_PARTICLES 200
Sprite *particles[NUM_PARTICLES];

void addParticle(int px, int py, int vx, int vy);
void updateAndDrawParticles();

bool init() 
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
				SDL_SetRenderDrawColor( gRenderer, 0x40, 0x00, 0x40, 0xFF );

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



int main( int argc, char* args[] )
{
   //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        SDL_Event e; 
        bool quit = false; 

        Sprite::setRenderer(gRenderer);

        // Simple IMAGE sprite
        s1 = new Sprite(gImagePath, 120);
        s1->setPos(100,100);
        s1->setVel(2,0);
        s1->setWrap(true);

        // animate it (2 images)
        s1->addAnimImage(gImagePath2);
        s1->setAnimTime(1000);

        // Sprite which has a shape but no image
        SDL_Color boxcol = {255, 50, 50, 255};
        s2 = new Sprite(SPRITE_SHAPE_RECT, 75, 75, &boxcol, 30);
        s2->setBorderColor(255,255,100);
        s2->setPos(200,200);
        s2->setVel(-3,2);
        s2->setWrap(true);
        s2->setLifetime(0, false);

        // Load a Sprite Sheet and create a sprite from it
        Sprite::loadSheet(gSpriteSheetPath);
        // {  7, 225, 16, 16} Invader1 pos1
        // { 40, 225, 16, 16} Invader1 pos2
        // { 74, 225, 22, 16} Invader2 pos1
        // {107, 225, 22, 16} Invader2 pos2
        // {147, 226, 24, 16} Invader3 pos1
        // {179, 226, 24, 16} Invader4 pos2

        SDL_Rect sspos;
        sspos = {7, 225, 16, 16};
        inv[0] = new Sprite(&sspos);
        sspos = {74, 225, 22, 16};
        inv[1] = new Sprite(&sspos);
        sspos = {147, 226, 24, 16};
        inv[2] = new Sprite(&sspos);
        sspos = {179, 226, 24, 16};
        inv[2]->addAnimSprite(&sspos);
        for (int i=0;i<3;i++) {
            inv[i]->setPos(50+50*i,300);
            inv[i]->setVel(1,0);
            inv[i]->setWrap(true);           
            inv[i]->setSpriteZoom(2); 
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
            SDL_SetRenderDrawColor( gRenderer, 0x40, 0x00, 0x40, 0xFF );

            //Clear screen
            SDL_RenderClear( gRenderer );

            s1->update();
            s1->draw();
            s2->update();
            s2->draw();
            for (int i=0;i<3;i++) {
                inv[i]->update();
                inv[i]->draw();
            }

            int mouseX, mouseY; Uint32 mouseButtonState;
            mouseButtonState = SDL_GetMouseState(&mouseX, &mouseY);
            if (SDL_BUTTON(SDL_BUTTON_LEFT) & mouseButtonState) {
                addParticle(mouseX, mouseY, rand()%10-5, rand()%10-5);
            }
            updateAndDrawParticles();

            //Update screen
            SDL_RenderPresent( gRenderer );
        }
    }
    close();

    return 0;
}

void close() 
{
    // delete sprite
    s1->destroy();

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    IMG_Quit();
    SDL_Quit();
}

void addParticle(int px, int py, int vx, int vy) {
    for(int i=0; i<NUM_PARTICLES; i++) {
        if (particles[i] == NULL) {
            particles[i] = new Sprite(SPRITE_SHAPE_CIRCLE, 20, 20, rand()%255, rand()%255, rand()%255, 255, 30);
            particles[i]->setPos(px, py);
            particles[i]->setVel(vx, vy);
            particles[i]->setLifetime(3000, true);
            particles[i]->setGravity(true);
            break;
        }
    }
}

void updateAndDrawParticles() {
    for(int i=0; i<NUM_PARTICLES; i++) {
        if (!particles[i]) continue;
        if (!particles[i]->update()) {
            delete particles[i]; particles[i] = NULL;
        } else {
            particles[i]->draw();
        }
    }
}
